/*
    Copyright (C) 2020 Sebastian J. Wolf

    This file is part of Fernweh.

    Fernweh is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Fernweh is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Fernweh. If not, see <http://www.gnu.org/licenses/>.
*/
#include "flickrapi.h"

#include <QStandardPaths>
#include <QFile>
#include <QDir>
#include <QImage>
#include <QProcess>

FlickrApi::FlickrApi(O1Requestor *requestor, QNetworkAccessManager *manager, QObject *parent) : QObject(parent)
{
    this->requestor = requestor;
    this->manager = manager;
}

FlickrApi::~FlickrApi()
{
}

void FlickrApi::testLogin()
{
    qDebug() << "FlickrApi::testLogin";
    QUrl url = QUrl(API_BASE_URL);
    QUrlQuery urlQuery = QUrlQuery();
    urlQuery.addQueryItem("method", "flickr.test.login");
    urlQuery.addQueryItem("format", "json");
    urlQuery.addQueryItem("nojsoncallback", "1");
    url.setQuery(urlQuery);
    QNetworkRequest request(url);

    QList<O0RequestParameter> requestParameters = QList<O0RequestParameter>();
    requestParameters.append(O0RequestParameter(QByteArray("method"), QByteArray("flickr.test.login")));
    requestParameters.append(O0RequestParameter(QByteArray("format"), QByteArray("json")));
    requestParameters.append(O0RequestParameter(QByteArray("nojsoncallback"), QByteArray("1")));
    QNetworkReply *reply = requestor->get(request, requestParameters);

    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(handleTestLoginError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(finished()), this, SLOT(handleTestLoginSuccessful()));
}

void FlickrApi::peopleGetInfo(const QString &userId)
{
    qDebug() << "FlickrApi::peopleGetInfo" << userId;
    QUrl url = QUrl(API_BASE_URL);
    QUrlQuery urlQuery = QUrlQuery();
    urlQuery.addQueryItem("method", "flickr.people.getInfo");
    urlQuery.addQueryItem("user_id", userId);
    urlQuery.addQueryItem("format", "json");
    urlQuery.addQueryItem("nojsoncallback", "1");
    url.setQuery(urlQuery);
    QNetworkRequest request(url);

    QList<O0RequestParameter> requestParameters = QList<O0RequestParameter>();
    requestParameters.append(O0RequestParameter(QByteArray("method"), QByteArray("flickr.people.getInfo")));
    requestParameters.append(O0RequestParameter(QByteArray("user_id"), userId.toUtf8()));
    requestParameters.append(O0RequestParameter(QByteArray("format"), QByteArray("json")));
    requestParameters.append(O0RequestParameter(QByteArray("nojsoncallback"), QByteArray("1")));
    QNetworkReply *reply = requestor->get(request, requestParameters);

    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(handlePeopleGetInfoError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(finished()), this, SLOT(handlePeopleGetInfoSuccessful()));

}

void FlickrApi::peopleGetPhotos(const QString &userId, const int &page)
{
    qDebug() << "FlickrApi::peopleGetPhotos" << userId;
    QUrl url = QUrl(API_BASE_URL);
    QUrlQuery urlQuery = QUrlQuery();
    urlQuery.addQueryItem("method", "flickr.people.getPhotos");
    urlQuery.addQueryItem("user_id", userId);
    urlQuery.addQueryItem("format", "json");
    urlQuery.addQueryItem("nojsoncallback", "1");
    urlQuery.addQueryItem("page", QString::number(page));
    urlQuery.addQueryItem("per_page", "200");
    urlQuery.addQueryItem("extras", "date_taken");
    url.setQuery(urlQuery);
    QNetworkRequest request(url);

    QList<O0RequestParameter> requestParameters = QList<O0RequestParameter>();
    requestParameters.append(O0RequestParameter(QByteArray("method"), QByteArray("flickr.people.getPhotos")));
    requestParameters.append(O0RequestParameter(QByteArray("user_id"), userId.toUtf8()));
    requestParameters.append(O0RequestParameter(QByteArray("format"), QByteArray("json")));
    requestParameters.append(O0RequestParameter(QByteArray("nojsoncallback"), QByteArray("1")));
    requestParameters.append(O0RequestParameter(QByteArray("page"), QString::number(page).toUtf8()));
    requestParameters.append(O0RequestParameter(QByteArray("per_page"), QByteArray("200")));
    requestParameters.append(O0RequestParameter(QByteArray("extras"), QByteArray("date_taken")));
    QNetworkReply *reply = requestor->get(request, requestParameters);

    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(handlePeopleGetPhotosError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(finished()), this, SLOT(handlePeopleGetPhotosSuccessful()));

}

void FlickrApi::downloadPhoto(const QString &farm, const QString &server, const QString &id, const QString &secret, const QString &size)
{
    qDebug() << "FlickrApi::downloadPhoto" << farm << server << id << secret << size;
    QUrl url = QUrl("https://farm" + farm + ".staticflickr.com/" + server + "/" + id + "_" + secret + "_" + size + ".jpg");
    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Wayland; SailfishOS) Fernweh (Not Firefox/42.0)");
    request.setRawHeader(QByteArray("Accept"), QByteArray("image/jpg,image/jpeg"));
    request.setRawHeader(QByteArray("Accept-Charset"), QByteArray("utf-8"));
    request.setRawHeader(QByteArray("Connection"), QByteArray("close"));
    request.setRawHeader(QByteArray("Cache-Control"), QByteArray("max-age=0"));
    request.setRawHeader(QByteArray(CUSTOM_HEADER_FARM), farm.toUtf8());
    request.setRawHeader(QByteArray(CUSTOM_HEADER_SERVER), server.toUtf8());
    request.setRawHeader(QByteArray(CUSTOM_HEADER_ID), id.toUtf8());
    request.setRawHeader(QByteArray(CUSTOM_HEADER_SECRET), secret.toUtf8());
    request.setRawHeader(QByteArray(CUSTOM_HEADER_SIZE), size.toUtf8());

    QString filePath = this->getCacheFilePath(farm, server, id, secret, size);

    if (QFile::exists(filePath)) {
        QVariantMap downloadIds = this->getDownloadIds(request);
        QImage downloadedImage(filePath);
        downloadIds.insert("width", downloadedImage.width());
        downloadIds.insert("height", downloadedImage.height());
        emit downloadSuccessful(downloadIds, filePath);
    } else {
        QNetworkReply *reply = manager->get(request);
        connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(handleDownloadError(QNetworkReply::NetworkError)));
        connect(reply, SIGNAL(finished()), this, SLOT(handleDownloadFinished()));
    }
}

void FlickrApi::downloadIcon(const QString &farm, const QString &server, const QString &id, const QString &iconKind)
{
    qDebug() << "FlickrApi::downloadIcon" << farm << server << id << iconKind;
    QString urlString = "https://farm" + farm + ".staticflickr.com/" + server + (iconKind == "buddy" ? "/buddyicons/" : "/coverphoto/" ) + id + (iconKind == "buddy" ? "_r" : "") + ".jpg";
    QUrl url = QUrl(urlString);
    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Wayland; SailfishOS) Fernweh (Not Firefox/42.0)");
    request.setRawHeader(QByteArray("Accept"), QByteArray("image/jpg,image/jpeg"));
    request.setRawHeader(QByteArray("Accept-Charset"), QByteArray("utf-8"));
    request.setRawHeader(QByteArray("Connection"), QByteArray("close"));
    request.setRawHeader(QByteArray("Cache-Control"), QByteArray("max-age=0"));
    request.setRawHeader(QByteArray(CUSTOM_HEADER_FARM), farm.toUtf8());
    request.setRawHeader(QByteArray(CUSTOM_HEADER_SERVER), server.toUtf8());
    request.setRawHeader(QByteArray(CUSTOM_HEADER_ID), id.toUtf8());
    request.setRawHeader(QByteArray(CUSTOM_HEADER_ICONKIND), iconKind.toUtf8());

    QString filePath = this->getCacheIconPath(farm, server, id, iconKind);

    if (QFile::exists(filePath)) {
        QVariantMap downloadIds = this->getDownloadIds(request);
        QImage downloadedImage(filePath);
        downloadIds.insert("width", downloadedImage.width());
        downloadIds.insert("height", downloadedImage.height());
        emit downloadIconSuccessful(downloadIds, filePath);
    } else {
        QNetworkReply *reply = manager->get(request);
        connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(handleDownloadIconError(QNetworkReply::NetworkError)));
        connect(reply, SIGNAL(finished()), this, SLOT(handleDownloadIconFinished()));
    }
}

void FlickrApi::copyPhotoToDownloads(const QString &farm, const QString &server, const QString &id, const QString &secret, const QString &size)
{
    QString downloadFilePath = this->getDownloadFilePath(id);
    QString downloadFileName = this->getDownloadFileName(id);
    QVariantMap downloadIds;
    downloadIds.insert("farm", farm);
    downloadIds.insert("server", server);
    downloadIds.insert("id", id);
    downloadIds.insert("secret", secret);
    downloadIds.insert("photoSize", size);
    if (QFile::copy(this->getCacheFilePath(farm, server, id, secret, size), downloadFilePath)) {
        emit copyToDownloadsSuccessful(downloadIds, downloadFileName, downloadFilePath);
    } else {
        emit copyToDownloadsError(downloadIds);
    }

}

void FlickrApi::statsGetTotalViews()
{
    qDebug() << "FlickrApi::statsGetTotalViews";
    QUrl url = QUrl(API_BASE_URL);
    QUrlQuery urlQuery = QUrlQuery();
    urlQuery.addQueryItem("method", "flickr.stats.getTotalViews");
    urlQuery.addQueryItem("format", "json");
    urlQuery.addQueryItem("nojsoncallback", "1");
    url.setQuery(urlQuery);
    QNetworkRequest request(url);

    QList<O0RequestParameter> requestParameters = QList<O0RequestParameter>();
    requestParameters.append(O0RequestParameter(QByteArray("method"), QByteArray("flickr.stats.getTotalViews")));
    requestParameters.append(O0RequestParameter(QByteArray("format"), QByteArray("json")));
    requestParameters.append(O0RequestParameter(QByteArray("nojsoncallback"), QByteArray("1")));
    QNetworkReply *reply = requestor->get(request, requestParameters);

    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(handleStatsGetTotalViewsError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(finished()), this, SLOT(handleStatsGetTotalViewsSuccessful()));
}

void FlickrApi::photosetsGetList(const QString &userId, const int &page)
{
    qDebug() << "FlickrApi::photosetsGetList" << userId;
    QUrl url = QUrl(API_BASE_URL);
    QUrlQuery urlQuery = QUrlQuery();
    urlQuery.addQueryItem("method", "flickr.photosets.getList");
    urlQuery.addQueryItem("user_id", userId);
    urlQuery.addQueryItem("format", "json");
    urlQuery.addQueryItem("nojsoncallback", "1");
    urlQuery.addQueryItem("page", QString::number(page));
    urlQuery.addQueryItem("per_page", "50");
    urlQuery.addQueryItem("extras", "date_taken");
    url.setQuery(urlQuery);
    QNetworkRequest request(url);

    QList<O0RequestParameter> requestParameters = QList<O0RequestParameter>();
    requestParameters.append(O0RequestParameter(QByteArray("method"), QByteArray("flickr.photosets.getList")));
    requestParameters.append(O0RequestParameter(QByteArray("user_id"), userId.toUtf8()));
    requestParameters.append(O0RequestParameter(QByteArray("format"), QByteArray("json")));
    requestParameters.append(O0RequestParameter(QByteArray("nojsoncallback"), QByteArray("1")));
    requestParameters.append(O0RequestParameter(QByteArray("page"), QString::number(page).toUtf8()));
    requestParameters.append(O0RequestParameter(QByteArray("per_page"), QByteArray("50")));
    requestParameters.append(O0RequestParameter(QByteArray("extras"), QByteArray("date_taken")));
    QNetworkReply *reply = requestor->get(request, requestParameters);

    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(handlePhotosetsGetListError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(finished()), this, SLOT(handlePhotosetsGetListSuccessful()));
}

void FlickrApi::photosetsGetPhotos(const QString &photosetId, const int &page)
{
    qDebug() << "FlickrApi::photosetsGetPhotos" << photosetId;
    QUrl url = QUrl(API_BASE_URL);
    QUrlQuery urlQuery = QUrlQuery();
    urlQuery.addQueryItem("method", "flickr.photosets.getPhotos");
    urlQuery.addQueryItem("photoset_id", photosetId);
    urlQuery.addQueryItem("format", "json");
    urlQuery.addQueryItem("nojsoncallback", "1");
    urlQuery.addQueryItem("page", QString::number(page));
    urlQuery.addQueryItem("per_page", "200");
    urlQuery.addQueryItem("extras", "date_taken");
    url.setQuery(urlQuery);
    QNetworkRequest request(url);

    QList<O0RequestParameter> requestParameters = QList<O0RequestParameter>();
    requestParameters.append(O0RequestParameter(QByteArray("method"), QByteArray("flickr.photosets.getPhotos")));
    requestParameters.append(O0RequestParameter(QByteArray("photoset_id"), photosetId.toUtf8()));
    requestParameters.append(O0RequestParameter(QByteArray("format"), QByteArray("json")));
    requestParameters.append(O0RequestParameter(QByteArray("nojsoncallback"), QByteArray("1")));
    requestParameters.append(O0RequestParameter(QByteArray("page"), QString::number(page).toUtf8()));
    requestParameters.append(O0RequestParameter(QByteArray("per_page"), QByteArray("200")));
    requestParameters.append(O0RequestParameter(QByteArray("extras"), QByteArray("date_taken")));
    QNetworkReply *reply = requestor->get(request, requestParameters);

    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(handlePhotosetsGetPhotosError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(finished()), this, SLOT(handlePhotosetsGetPhotosSuccessful()));
}

void FlickrApi::interestingnessGetList(const int &page)
{
    qDebug() << "FlickrApi::interestingnessGetList";
    QUrl url = QUrl(API_BASE_URL);
    QUrlQuery urlQuery = QUrlQuery();
    urlQuery.addQueryItem("method", "flickr.interestingness.getList");
    urlQuery.addQueryItem("format", "json");
    urlQuery.addQueryItem("nojsoncallback", "1");
    urlQuery.addQueryItem("page", QString::number(page));
    urlQuery.addQueryItem("per_page", "200");
    urlQuery.addQueryItem("extras", "date_taken");
    url.setQuery(urlQuery);
    QNetworkRequest request(url);

    QList<O0RequestParameter> requestParameters = QList<O0RequestParameter>();
    requestParameters.append(O0RequestParameter(QByteArray("method"), QByteArray("flickr.interestingness.getList")));
    requestParameters.append(O0RequestParameter(QByteArray("format"), QByteArray("json")));
    requestParameters.append(O0RequestParameter(QByteArray("nojsoncallback"), QByteArray("1")));
    requestParameters.append(O0RequestParameter(QByteArray("page"), QString::number(page).toUtf8()));
    requestParameters.append(O0RequestParameter(QByteArray("per_page"), QByteArray("200")));
    requestParameters.append(O0RequestParameter(QByteArray("extras"), QByteArray("date_taken")));
    QNetworkReply *reply = requestor->get(request, requestParameters);

    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(handleInterestingnessGetListError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(finished()), this, SLOT(handleInterestingnessGetListSuccessful()));
}

void FlickrApi::photosSearch(const QString &searchString, const int &page)
{
    qDebug() << "FlickrApi::photosSearch" << searchString;
    QUrl url = QUrl(API_BASE_URL);
    QUrlQuery urlQuery = QUrlQuery();
    urlQuery.addQueryItem("method", "flickr.photos.search");
    urlQuery.addQueryItem("format", "json");
    urlQuery.addQueryItem("nojsoncallback", "1");
    urlQuery.addQueryItem("text", searchString);
    urlQuery.addQueryItem("page", QString::number(page));
    urlQuery.addQueryItem("per_page", "200");
    urlQuery.addQueryItem("extras", "date_taken");
    url.setQuery(urlQuery);
    QNetworkRequest request(url);

    QList<O0RequestParameter> requestParameters = QList<O0RequestParameter>();
    requestParameters.append(O0RequestParameter(QByteArray("method"), QByteArray("flickr.photos.search")));
    requestParameters.append(O0RequestParameter(QByteArray("format"), QByteArray("json")));
    requestParameters.append(O0RequestParameter(QByteArray("nojsoncallback"), QByteArray("1")));
    requestParameters.append(O0RequestParameter(QByteArray("text"), searchString.toUtf8()));
    requestParameters.append(O0RequestParameter(QByteArray("page"), QString::number(page).toUtf8()));
    requestParameters.append(O0RequestParameter(QByteArray("per_page"), QByteArray("200")));
    requestParameters.append(O0RequestParameter(QByteArray("extras"), QByteArray("date_taken")));
    QNetworkReply *reply = requestor->get(request, requestParameters);

    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(handlePhotosSearchError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(finished()), this, SLOT(handlePhotosSearchSuccessful()));
}

void FlickrApi::photosGetInfo(const QString &photoId)
{
    qDebug() << "FlickrApi::photosGetInfo" << photoId;
    QUrl url = QUrl(API_BASE_URL);
    QUrlQuery urlQuery = QUrlQuery();
    urlQuery.addQueryItem("method", "flickr.photos.getInfo");
    urlQuery.addQueryItem("photo_id", photoId);
    urlQuery.addQueryItem("format", "json");
    urlQuery.addQueryItem("nojsoncallback", "1");
    url.setQuery(urlQuery);
    QNetworkRequest request(url);

    QList<O0RequestParameter> requestParameters = QList<O0RequestParameter>();
    requestParameters.append(O0RequestParameter(QByteArray("method"), QByteArray("flickr.photos.getInfo")));
    requestParameters.append(O0RequestParameter(QByteArray("photo_id"), photoId.toUtf8()));
    requestParameters.append(O0RequestParameter(QByteArray("format"), QByteArray("json")));
    requestParameters.append(O0RequestParameter(QByteArray("nojsoncallback"), QByteArray("1")));
    QNetworkReply *reply = requestor->get(request, requestParameters);

    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(handlePhotosGetInfoError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(finished()), this, SLOT(handlePhotosGetInfoSuccessful()));
}

void FlickrApi::photosGetExif(const QString &photoId, const QString &secret)
{
    qDebug() << "FlickrApi::photosGetExif" << photoId << secret;
    QUrl url = QUrl(API_BASE_URL);
    QUrlQuery urlQuery = QUrlQuery();
    urlQuery.addQueryItem("method", "flickr.photos.getExif");
    urlQuery.addQueryItem("photo_id", photoId);
    urlQuery.addQueryItem("secret", secret);
    urlQuery.addQueryItem("format", "json");
    urlQuery.addQueryItem("nojsoncallback", "1");
    url.setQuery(urlQuery);
    QNetworkRequest request(url);

    QList<O0RequestParameter> requestParameters = QList<O0RequestParameter>();
    requestParameters.append(O0RequestParameter(QByteArray("method"), QByteArray("flickr.photos.getExif")));
    requestParameters.append(O0RequestParameter(QByteArray("photo_id"), photoId.toUtf8()));
    requestParameters.append(O0RequestParameter(QByteArray("secret"), secret.toUtf8()));
    requestParameters.append(O0RequestParameter(QByteArray("format"), QByteArray("json")));
    requestParameters.append(O0RequestParameter(QByteArray("nojsoncallback"), QByteArray("1")));
    QNetworkReply *reply = requestor->get(request, requestParameters);

    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(handlePhotosGetExifError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(finished()), this, SLOT(handlePhotosGetExifSuccessful()));
}

void FlickrApi::photosGetFavorites(const QString &photoId)
{
    qDebug() << "FlickrApi::photosGetFavorites" << photoId;
    QUrl url = QUrl(API_BASE_URL);
    QUrlQuery urlQuery = QUrlQuery();
    urlQuery.addQueryItem("method", "flickr.photos.getFavorites");
    urlQuery.addQueryItem("photo_id", photoId);
    urlQuery.addQueryItem("format", "json");
    urlQuery.addQueryItem("nojsoncallback", "1");
    urlQuery.addQueryItem("page", "1");
    urlQuery.addQueryItem("per_page", "5");
    url.setQuery(urlQuery);
    QNetworkRequest request(url);

    QList<O0RequestParameter> requestParameters = QList<O0RequestParameter>();
    requestParameters.append(O0RequestParameter(QByteArray("method"), QByteArray("flickr.photos.getExif")));
    requestParameters.append(O0RequestParameter(QByteArray("photo_id"), photoId.toUtf8()));
    requestParameters.append(O0RequestParameter(QByteArray("format"), QByteArray("json")));
    requestParameters.append(O0RequestParameter(QByteArray("nojsoncallback"), QByteArray("1")));
    requestParameters.append(O0RequestParameter(QByteArray("page"), QByteArray("1")));
    requestParameters.append(O0RequestParameter(QByteArray("per_page"), QByteArray("5")));
    QNetworkReply *reply = requestor->get(request, requestParameters);

    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(handlePhotosGetFavoritesError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(finished()), this, SLOT(handlePhotosGetFavoritesSuccessful()));
}

void FlickrApi::photosLicensesGetInfo()
{
    qDebug() << "FlickrApi::photosLicensesGetInfo";
    QUrl url = QUrl(API_BASE_URL);
    QUrlQuery urlQuery = QUrlQuery();
    urlQuery.addQueryItem("method", "flickr.photos.licenses.getInfo");
    urlQuery.addQueryItem("format", "json");
    urlQuery.addQueryItem("nojsoncallback", "1");
    url.setQuery(urlQuery);
    QNetworkRequest request(url);

    QList<O0RequestParameter> requestParameters = QList<O0RequestParameter>();
    requestParameters.append(O0RequestParameter(QByteArray("method"), QByteArray("flickr.photos.licenses.getInfo")));
    requestParameters.append(O0RequestParameter(QByteArray("format"), QByteArray("json")));
    requestParameters.append(O0RequestParameter(QByteArray("nojsoncallback"), QByteArray("1")));
    QNetworkReply *reply = requestor->get(request, requestParameters);

    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(handlePhotosLicensesGetInfoError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(finished()), this, SLOT(handlePhotosLicensesGetInfoSuccessful()));
}

void FlickrApi::handleAdditionalInformation(const QString &additionalInformation)
{
    qDebug() << "FlickrApi::handleAdditionalInformation" << additionalInformation;
    // For now only used to open downloaded files...
    QStringList argumentsList;
    argumentsList.append(additionalInformation);
    bool successfullyStarted = QProcess::startDetached("xdg-open", argumentsList);
    if (successfullyStarted) {
        qDebug() << "Successfully opened file " << additionalInformation;
    } else {
        qDebug() << "Error opening file " << additionalInformation;
    }
}

void FlickrApi::handleTestLoginSuccessful()
{
    qDebug() << "FlickrApi::handleTestLoginSuccessful";
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError) {
        return;
    }

    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll());
    if (jsonDocument.isObject()) {
        emit testLoginSuccessful(jsonDocument.object().toVariantMap());
    } else {
        emit testLoginError("Fernweh couldn't understand Flickr's response!");
    }
}

void FlickrApi::handleTestLoginError(QNetworkReply::NetworkError error)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    qWarning() << "FlickrApi::handleTestLoginError:" << (int)error << reply->errorString();
    emit testLoginError(reply->errorString());
}

void FlickrApi::handlePeopleGetInfoSuccessful()
{
    qDebug() << "FlickrApi::handlePeopleGetInfoSuccessful";
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError) {
        return;
    }

    QString urlQueryRaw = reply->request().url().query();
    QUrlQuery urlQuery(urlQueryRaw);

    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll());
    // qDebug().noquote() << jsonDocument.toJson();
    if (jsonDocument.isObject()) {
        emit peopleGetInfoSuccessful(urlQuery.queryItemValue("user_id"), jsonDocument.object().toVariantMap());
    } else {
        emit peopleGetInfoError(urlQuery.queryItemValue("user_id"), "Fernweh couldn't understand Flickr's response!");
    }
}

void FlickrApi::handlePeopleGetInfoError(QNetworkReply::NetworkError error)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    qWarning() << "FlickrApi::handlePeopleGetInfoError:" << (int)error << reply->errorString();
    QString urlQueryRaw = reply->request().url().query();
    QUrlQuery urlQuery(urlQueryRaw);
    emit peopleGetInfoError(urlQuery.queryItemValue("user_id"), reply->errorString());
}

void FlickrApi::handlePeopleGetPhotosSuccessful()
{
    qDebug() << "FlickrApi::handlePeopleGetPhotosSuccessful";
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError) {
        return;
    }

    QString urlQueryRaw = reply->request().url().query();
    QUrlQuery urlQuery(urlQueryRaw);

    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll());

    //qDebug().noquote() << jsonDocument.toJson();

    if (jsonDocument.isObject()) {
        int page = urlQuery.queryItemValue("page").toInt();
        if (urlQuery.queryItemValue("user_id") == "me") {
            emit ownPhotosSuccessful(jsonDocument.object().toVariantMap(), page > 1 ? true : false);
        } else {
            emit peopleGetPhotosSuccessful(urlQuery.queryItemValue("user_id"), jsonDocument.object().toVariantMap());
        }
    } else {
        if (urlQuery.queryItemValue("user_id") == "me") {
            emit ownPhotosError("Fernweh couldn't understand Flickr's response!");
        } else {
            emit peopleGetPhotosError(urlQuery.queryItemValue("user_id"), "Fernweh couldn't understand Flickr's response!");
        }
    }
}

void FlickrApi::handlePeopleGetPhotosError(QNetworkReply::NetworkError error)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    qWarning() << "FlickrApi::handlePeopleGetPhotosError:" << (int)error << reply->errorString();
    QString urlQueryRaw = reply->request().url().query();
    QUrlQuery urlQuery(urlQueryRaw);
    if (urlQuery.queryItemValue("user_id") == "me") {
        emit ownPhotosError(reply->errorString());
    } else {
        emit peopleGetPhotosError(urlQuery.queryItemValue("user_id"), reply->errorString());
    }
}

void FlickrApi::handleDownloadError(QNetworkReply::NetworkError error)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    qWarning() << "FlickrApi::handleDownloadError:" << (int)error << reply->errorString();
    emit downloadError(this->getDownloadIds(reply->request()), reply->errorString());
}

void FlickrApi::handleDownloadFinished()
{
    qDebug() << "FlickrApi::handleDownloadFinished";
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError) {
        return;
    }

    QVariantMap downloadIds = this->getDownloadIds(reply->request());

    QDir cachePath(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
    if (!cachePath.exists()) {
        cachePath.mkpath(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
    }

    QString filePath = this->getCacheFilePath(downloadIds.value("farm").toString(),
                                                 downloadIds.value("server").toString(),
                                                 downloadIds.value("id").toString(),
                                                 downloadIds.value("secret").toString(),
                                                 downloadIds.value("photoSize").toString());

    QFile downloadedFile(filePath);
    if (downloadedFile.open(QIODevice::WriteOnly)) {
        qDebug() << "Writing downloaded file to " + downloadedFile.fileName();
        downloadedFile.write(reply->readAll());
        QImage downloadedImage(filePath);
        downloadIds.insert("width", downloadedImage.width());
        downloadIds.insert("height", downloadedImage.height());
        downloadedFile.close();
        emit downloadSuccessful(downloadIds, filePath);
    } else {
        emit downloadError(downloadIds, "Error storing file at " + filePath + ", error was: " + downloadedFile.errorString());
    }
}

void FlickrApi::handleDownloadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError) {
        return;
    }

    QVariantMap downloadIds = this->getDownloadIds(reply->request());

    int percentCompleted = 100 * bytesSent / bytesTotal;
    emit downloadStatus(downloadIds, percentCompleted);
}

void FlickrApi::handleDownloadIconError(QNetworkReply::NetworkError error)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    qWarning() << "FlickrApi::handleDownloadIconError:" << (int)error << reply->errorString();
    emit downloadIconError(this->getDownloadIds(reply->request()), reply->errorString());
}

void FlickrApi::handleDownloadIconFinished()
{
    qDebug() << "FlickrApi::handleDownloadIconFinished";
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError) {
        return;
    }

    QVariantMap downloadIds = this->getDownloadIds(reply->request());

    QDir cachePath(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
    if (!cachePath.exists()) {
        cachePath.mkpath(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
    }

    QString filePath = this->getCacheIconPath(downloadIds.value("farm").toString(),
                                              downloadIds.value("server").toString(),
                                              downloadIds.value("id").toString(),
                                              downloadIds.value("iconKind").toString());

    QFile downloadedFile(filePath);
    if (downloadedFile.open(QIODevice::WriteOnly)) {
        qDebug() << "Writing downloaded icon to " + downloadedFile.fileName();
        downloadedFile.write(reply->readAll());
        QImage downloadedImage(filePath);
        downloadIds.insert("width", downloadedImage.width());
        downloadIds.insert("height", downloadedImage.height());
        downloadedFile.close();
        emit downloadIconSuccessful(downloadIds, filePath);
    } else {
        emit downloadIconError(downloadIds, "Error storing icon at " + filePath + ", error was: " + downloadedFile.errorString());
    }
}

void FlickrApi::handleStatsGetTotalViewsSuccessful()
{
    qDebug() << "FlickrApi::handleStatsGetTotalViewsSuccessful";
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError) {
        return;
    }

    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll());
    // qDebug().noquote() << jsonDocument.toJson();
    if (jsonDocument.isObject()) {
        emit statsGetTotalViewsSuccessful(jsonDocument.object().toVariantMap());
    } else {
        emit statsGetTotalViewsError("Fernweh couldn't understand Flickr's response!");
    }
}

void FlickrApi::handleStatsGetTotalViewsError(QNetworkReply::NetworkError error)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    qWarning() << "FlickrApi::handleStatsGetTotalViewsError:" << (int)error << reply->errorString();
    emit statsGetTotalViewsError(reply->errorString());
}

void FlickrApi::handlePhotosetsGetListSuccessful()
{
    qDebug() << "FlickrApi::handlePhotosetsGetListSuccessful";
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError) {
        return;
    }

    QString urlQueryRaw = reply->request().url().query();
    QUrlQuery urlQuery(urlQueryRaw);

    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll());

    //qDebug().noquote() << jsonDocument.toJson();

    if (jsonDocument.isObject()) {
        int page = urlQuery.queryItemValue("page").toInt();
        if (urlQuery.queryItemValue("user_id").isEmpty()) {
            emit ownPhotosetsSuccessful(jsonDocument.object().toVariantMap(), page > 1 ? true : false);
        } else {
            emit photosetsGetListSuccessful(urlQuery.queryItemValue("user_id"), jsonDocument.object().toVariantMap());
        }
    } else {
        if (urlQuery.queryItemValue("user_id").isEmpty()) {
            emit ownPhotosetsError("Fernweh couldn't understand Flickr's response!");
        } else {
            emit photosetsGetListError(urlQuery.queryItemValue("user_id"), "Fernweh couldn't understand Flickr's response!");
        }
    }
}

void FlickrApi::handlePhotosetsGetListError(QNetworkReply::NetworkError error)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    qWarning() << "FlickrApi::handlePhotosetsGetListError:" << (int)error << reply->errorString();
    QString urlQueryRaw = reply->request().url().query();
    QUrlQuery urlQuery(urlQueryRaw);
    if (urlQuery.queryItemValue("user_id").isEmpty()) {
        emit ownPhotosetsError(reply->errorString());
    } else {
        emit photosetsGetListError(urlQuery.queryItemValue("user_id"), reply->errorString());
    }
}

void FlickrApi::handlePhotosetsGetPhotosSuccessful()
{
    qDebug() << "FlickrApi::handlePhotosetsGetPhotosError";
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError) {
        return;
    }

    QString urlQueryRaw = reply->request().url().query();
    QUrlQuery urlQuery(urlQueryRaw);

    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll());

    // sqDebug().noquote() << jsonDocument.toJson();

    if (jsonDocument.isObject()) {
        emit photosetsGetPhotosSuccessful(urlQuery.queryItemValue("photoset_id"), jsonDocument.object().toVariantMap());
    } else {
        emit photosetsGetPhotosError(urlQuery.queryItemValue("photoset_id"), "Fernweh couldn't understand Flickr's response!");
    }
}

void FlickrApi::handlePhotosetsGetPhotosError(QNetworkReply::NetworkError error)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    qWarning() << "FlickrApi::handlePhotosetsGetPhotosSuccessful:" << (int)error << reply->errorString();
    QString urlQueryRaw = reply->request().url().query();
    QUrlQuery urlQuery(urlQueryRaw);
    emit photosetsGetPhotosError(urlQuery.queryItemValue("photoset_id"), reply->errorString());
}

void FlickrApi::handleInterestingnessGetListSuccessful()
{
    qDebug() << "FlickrApi::handleInterestingnessGetListSuccessful";
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError) {
        return;
    }

    QString urlQueryRaw = reply->request().url().query();
    QUrlQuery urlQuery(urlQueryRaw);

    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll());

    // qDebug().noquote() << jsonDocument.toJson();

    if (jsonDocument.isObject()) {
        int page = urlQuery.queryItemValue("page").toInt();
        emit interestingnessGetListSuccessful(jsonDocument.object().toVariantMap(), page > 1 ? true : false);
    } else {
        emit interestingnessGetListError("Fernweh couldn't understand Flickr's response!");
    }
}

void FlickrApi::handleInterestingnessGetListError(QNetworkReply::NetworkError error)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    qWarning() << "FlickrApi::handleInterestingnessGetListError:" << (int)error << reply->errorString();
    emit interestingnessGetListError(reply->errorString());
}

void FlickrApi::handlePhotosSearchSuccessful()
{
    qDebug() << "FlickrApi::handlePhotosSearchSuccessful";
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError) {
        return;
    }

    QString urlQueryRaw = reply->request().url().query();
    QUrlQuery urlQuery(urlQueryRaw);

    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll());

    // qDebug().noquote() << jsonDocument.toJson();

    if (jsonDocument.isObject()) {
        int page = urlQuery.queryItemValue("page").toInt();
        emit photosSearchSuccessful(jsonDocument.object().toVariantMap(), page > 1 ? true : false);
    } else {
        emit photosSearchError("Fernweh couldn't understand Flickr's response!");
    }
}

void FlickrApi::handlePhotosSearchError(QNetworkReply::NetworkError error)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    qWarning() << "FlickrApi::handlePhotosSearchError:" << (int)error << reply->errorString();
    emit photosSearchError(reply->errorString());
}

void FlickrApi::handlePhotosGetInfoSuccessful()
{
    qDebug() << "FlickrApi::handlePhotosGetInfoSuccessful";
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError) {
        return;
    }

    QString urlQueryRaw = reply->request().url().query();
    QUrlQuery urlQuery(urlQueryRaw);

    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll());

    // qDebug().noquote() << jsonDocument.toJson();

    if (jsonDocument.isObject()) {
        emit photosGetInfoSuccessful(urlQuery.queryItemValue("photo_id"), jsonDocument.object().toVariantMap());
    } else {
        emit photosGetInfoError(urlQuery.queryItemValue("photo_id"), "Fernweh couldn't understand Flickr's response!");
    }
}

void FlickrApi::handlePhotosGetInfoError(QNetworkReply::NetworkError error)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    qWarning() << "FlickrApi::handlePhotosGetInfoError:" << (int)error << reply->errorString();
    QString urlQueryRaw = reply->request().url().query();
    QUrlQuery urlQuery(urlQueryRaw);
    emit photosGetInfoError(urlQuery.queryItemValue("photo_id"), reply->errorString());
}

void FlickrApi::handlePhotosGetExifSuccessful()
{
    qDebug() << "FlickrApi::handlePhotosGetExifSuccessful";
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError) {
        return;
    }

    QString urlQueryRaw = reply->request().url().query();
    QUrlQuery urlQuery(urlQueryRaw);

    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll());

    // qDebug().noquote() << jsonDocument.toJson();

    if (jsonDocument.isObject()) {
        emit photosGetExifSuccessful(urlQuery.queryItemValue("photo_id"), jsonDocument.object().toVariantMap());
    } else {
        emit photosGetExifError(urlQuery.queryItemValue("photo_id"), "Fernweh couldn't understand Flickr's response!");
    }
}

void FlickrApi::handlePhotosGetExifError(QNetworkReply::NetworkError error)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    qWarning() << "FlickrApi::handlePhotosGetExifError:" << (int)error << reply->errorString();
    QString urlQueryRaw = reply->request().url().query();
    QUrlQuery urlQuery(urlQueryRaw);
    emit photosGetExifError(urlQuery.queryItemValue("photo_id"), reply->errorString());
}

void FlickrApi::handlePhotosGetFavoritesSuccessful()
{
    qDebug() << "FlickrApi::handlePhotosGetFavoritesSuccessful";
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError) {
        return;
    }

    QString urlQueryRaw = reply->request().url().query();
    QUrlQuery urlQuery(urlQueryRaw);

    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll());

    // qDebug().noquote() << jsonDocument.toJson();

    if (jsonDocument.isObject()) {
        emit photosGetFavoritesSuccessful(urlQuery.queryItemValue("photo_id"), jsonDocument.object().toVariantMap());
    } else {
        emit photosGetFavoritesError(urlQuery.queryItemValue("photo_id"), "Fernweh couldn't understand Flickr's response!");
    }
}

void FlickrApi::handlePhotosGetFavoritesError(QNetworkReply::NetworkError error)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    qWarning() << "FlickrApi::handlePhotosGetFavoritesError:" << (int)error << reply->errorString();
    QString urlQueryRaw = reply->request().url().query();
    QUrlQuery urlQuery(urlQueryRaw);
    emit photosGetFavoritesError(urlQuery.queryItemValue("photo_id"), reply->errorString());
}

void FlickrApi::handlePhotosLicensesGetInfoSuccessful()
{
    qDebug() << "FlickrApi::handlePhotosLicensesGetInfoSuccessful";
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError) {
        return;
    }

    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll());

    //qDebug().noquote() << jsonDocument.toJson();

    if (jsonDocument.isObject()) {
        emit photosLicensesGetInfoSuccessful(jsonDocument.object().toVariantMap());
    } else {
        emit photosLicensesGetInfoError("Fernweh couldn't understand Flickr's response!");
    }
}

void FlickrApi::handlePhotosLicensesGetInfoError(QNetworkReply::NetworkError error)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    qWarning() << "FlickrApi::handlePhotosLicensesGetInfoError:" << (int)error << reply->errorString();
    emit photosLicensesGetInfoError(reply->errorString());
}

QVariantMap FlickrApi::getDownloadIds(const QNetworkRequest &request)
{
    QVariantMap downloadIds;
    downloadIds.insert("farm", QString::fromUtf8(request.rawHeader(CUSTOM_HEADER_FARM)));
    downloadIds.insert("server", QString::fromUtf8(request.rawHeader(CUSTOM_HEADER_SERVER)));
    downloadIds.insert("id", QString::fromUtf8(request.rawHeader(CUSTOM_HEADER_ID)));
    if (request.hasRawHeader(CUSTOM_HEADER_SECRET)) {
        downloadIds.insert("secret", QString::fromUtf8(request.rawHeader(CUSTOM_HEADER_SECRET)));
    }
    if (request.hasRawHeader(CUSTOM_HEADER_SIZE)) {
        downloadIds.insert("photoSize", QString::fromUtf8(request.rawHeader(CUSTOM_HEADER_SIZE)));
    }
    if (request.hasRawHeader(CUSTOM_HEADER_ICONKIND)) {
        downloadIds.insert("iconKind", QString::fromUtf8(request.rawHeader(CUSTOM_HEADER_ICONKIND)));
    }
    return downloadIds;
}

QString FlickrApi::getCacheFilePath(const QString &farm, const QString &server, const QString &id, const QString &secret, const QString &size)
{
    QString filePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + QLatin1Char('/') + farm + "_"
                                                                                                          + server + "_"
                                                                                                          + id + "_"
                                                                                                          + secret + "_"
                                                                                                          + size + ".jpg";
    return filePath;
}

QString FlickrApi::getCacheIconPath(const QString &farm, const QString &server, const QString &id, const QString &iconKind)
{
    QString filePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + QLatin1Char('/') + farm + "_"
                                                                                                          + server + "_"
                                                                                                          + id + "_"
                                                                                                          + iconKind + ".jpg";
    return filePath;
}

QString FlickrApi::getDownloadFilePath(const QString &id)
{
    QString filePath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + QLatin1Char('/') + this->getDownloadFileName(id);
    return filePath;
}

QString FlickrApi::getDownloadFileName(const QString &id)
{
    return "flickr_" + id + ".jpg";
}
