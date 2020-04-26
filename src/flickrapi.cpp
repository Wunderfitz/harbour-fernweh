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

FlickrApi::FlickrApi(O1Requestor *requestor, QNetworkAccessManager *manager, QObject *parent) : QObject(parent)
{
    this->requestor = requestor;
    this->manager = manager;
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

void FlickrApi::peopleGetPhotos(const QString &userId)
{
    qDebug() << "FlickrApi::peopleGetPhotos" << userId;
    QUrl url = QUrl(API_BASE_URL);
    QUrlQuery urlQuery = QUrlQuery();
    urlQuery.addQueryItem("method", "flickr.people.getPhotos");
    urlQuery.addQueryItem("user_id", userId);
    urlQuery.addQueryItem("format", "json");
    urlQuery.addQueryItem("nojsoncallback", "1");
    urlQuery.addQueryItem("per_page", "50");
    url.setQuery(urlQuery);
    QNetworkRequest request(url);

    QList<O0RequestParameter> requestParameters = QList<O0RequestParameter>();
    requestParameters.append(O0RequestParameter(QByteArray("method"), QByteArray("flickr.people.getPhotos")));
    requestParameters.append(O0RequestParameter(QByteArray("user_id"), userId.toUtf8()));
    requestParameters.append(O0RequestParameter(QByteArray("format"), QByteArray("json")));
    requestParameters.append(O0RequestParameter(QByteArray("nojsoncallback"), QByteArray("1")));
    requestParameters.append(O0RequestParameter(QByteArray("per_page"), QByteArray("50")));
    QNetworkReply *reply = requestor->get(request, requestParameters);

    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(handlePeopleGetPhotosError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(finished()), this, SLOT(handlePeopleGetPhotosSuccessful()));

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
    qDebug().noquote() << jsonDocument.toJson();
    if (jsonDocument.isObject()) {
        if (urlQuery.queryItemValue("user_id") == "me") {
            emit ownPhotosSuccessful(jsonDocument.object().toVariantMap(), false);
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
