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

class FlickrApi : public QObject
{
    Q_OBJECT
public:
    explicit FlickrApi(O1Requestor* requestor, QNetworkAccessManager *manager, QObject *parent = nullptr);

    Q_INVOKABLE void testLogin();
    Q_INVOKABLE void peopleGetInfo(const QString &userId);
    Q_INVOKABLE void peopleGetPhotos(const QString &userId);

signals:
    void testLoginSuccessful(const QVariantMap &result);
    void testLoginError(const QString &errorMessage);
    void peopleGetInfoSuccessful(const QString userId, const QVariantMap &result);
    void peopleGetInfoError(const QString userId, const QString &errorMessage);
    void peopleGetPhotosSuccessful(const QString userId, const QVariantMap &result);
    void peopleGetPhotosError(const QString userId, const QString &errorMessage);
    void ownPhotosSuccessful(const QVariantMap &result, const bool incrementalUpdate);
    void ownPhotosError(const QString &errorMessage);

public slots:
    void handleTestLoginSuccessful();
    void handleTestLoginError(QNetworkReply::NetworkError error);
    void handlePeopleGetInfoSuccessful();
    void handlePeopleGetInfoError(QNetworkReply::NetworkError error);
    void handlePeopleGetPhotosSuccessful();
    void handlePeopleGetPhotosError(QNetworkReply::NetworkError error);

private:
    O1Requestor *requestor;
    QNetworkAccessManager *manager;
};

#endif // FLICKRAPI_H
