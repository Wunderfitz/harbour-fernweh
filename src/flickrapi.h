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
#ifndef FLICKRAPI_H
#define FLICKRAPI_H

#include <QObject>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariantMap>
#include <QVariantList>
#include <QNetworkAccessManager>
#include "o1requestor.h"

const char API_BASE_URL[] = "https://www.flickr.com/services/rest";
const char CUSTOM_HEADER_FARM[] = "X-Fernweh-Farm";
const char CUSTOM_HEADER_SERVER[] = "X-Fernweh-Server";
const char CUSTOM_HEADER_ID[] = "X-Fernweh-Id";
const char CUSTOM_HEADER_SECRET[] = "X-Fernweh-Secret";
const char CUSTOM_HEADER_SIZE[] = "X-Fernweh-Size";

class FlickrApi : public QObject
{
    Q_OBJECT
public:
    explicit FlickrApi(O1Requestor* requestor, QNetworkAccessManager *manager, QObject *parent = nullptr);
    ~FlickrApi();

    Q_INVOKABLE void testLogin();
    Q_INVOKABLE void peopleGetInfo(const QString &userId);
    Q_INVOKABLE void peopleGetPhotos(const QString &userId, const int &page = 1);
    Q_INVOKABLE void downloadPhoto(const QString &farm, const QString &server, const QString &id, const QString &secret, const QString &size);
    Q_INVOKABLE void copyPhotoToDownloads(const QString &farm, const QString &server, const QString &id, const QString &secret, const QString &size);
    Q_INVOKABLE void statsGetTotalViews();
    Q_INVOKABLE void photosetsGetList(const QString &userId, const int &page = 1);
    Q_INVOKABLE void photosetsGetPhotos(const QString &photosetId, const int &page = 1);

signals:
    void testLoginSuccessful(const QVariantMap &result);
    void testLoginError(const QString &errorMessage);
    void peopleGetInfoSuccessful(const QString userId, const QVariantMap &result);
    void peopleGetInfoError(const QString userId, const QString &errorMessage);
    void peopleGetPhotosSuccessful(const QString userId, const QVariantMap &result);
    void peopleGetPhotosError(const QString userId, const QString &errorMessage);
    void ownPhotosSuccessful(const QVariantMap &result, const bool incrementalUpdate);
    void ownPhotosError(const QString &errorMessage);
    void downloadError(const QVariantMap &downloadIds, const QString &errorMessage);
    void downloadSuccessful(const QVariantMap &downloadIds, const QString &filePath);
    void downloadStatus(const QVariantMap &downloadIds, int percentCompleted);
    void copyToDownloadsSuccessful(const QVariantMap &downloadIds, const QString &fileName, const QString &filePath);
    void copyToDownloadsError(const QVariantMap &downloadIds);
    void statsGetTotalViewsSuccessful(const QVariantMap &result);
    void statsGetTotalViewsError(const QString &errorMessage);
    void photosetsGetListSuccessful(const QString userId, const QVariantMap &result);
    void photosetsGetListError(const QString userId, const QString &errorMessage);
    void ownPhotosetsSuccessful(const QVariantMap &result, const bool incrementalUpdate);
    void ownPhotosetsError(const QString &errorMessage);
    void photosetsGetPhotosSuccessful(const QString photosetId, const QVariantMap &result);
    void photosetsGetPhotosError(const QString photosetId, const QString &errorMessage);

public slots:
    void handleTestLoginSuccessful();
    void handleTestLoginError(QNetworkReply::NetworkError error);
    void handlePeopleGetInfoSuccessful();
    void handlePeopleGetInfoError(QNetworkReply::NetworkError error);
    void handlePeopleGetPhotosSuccessful();
    void handlePeopleGetPhotosError(QNetworkReply::NetworkError error);
    void handleDownloadError(QNetworkReply::NetworkError error);
    void handleDownloadFinished();
    void handleDownloadProgress(qint64 bytesSent, qint64 bytesTotal);
    void handleStatsGetTotalViewsSuccessful();
    void handleStatsGetTotalViewsError(QNetworkReply::NetworkError error);
    void handlePhotosetsGetListSuccessful();
    void handlePhotosetsGetListError(QNetworkReply::NetworkError error);
    void handlePhotosetsGetPhotosSuccessful();
    void handlePhotosetsGetPhotosError(QNetworkReply::NetworkError error);

private:
    QVariantMap getDownloadIds(const QNetworkRequest &request);
    QString getCacheFilePath(const QString &farm, const QString &server, const QString &id, const QString &secret, const QString &size);
    QString getDownloadFilePath(const QString &id);
    QString getDownloadFileName(const QString &id);

    O1Requestor *requestor;
    QNetworkAccessManager *manager;
};

#endif // FLICKRAPI_H
