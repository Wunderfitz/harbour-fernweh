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
