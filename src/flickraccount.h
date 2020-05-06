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

#ifndef FLICKRACCOUNT_H
#define FLICKRACCOUNT_H

#include <QObject>
#include <QNetworkConfigurationManager>
#include <QNetworkAccessManager>
#include <QSettings>

#include "o1flickr.h"
#include "o1requestor.h"
#include "flickrapi.h"

class FlickrAccount : public QObject
{
    Q_OBJECT
public:
    explicit FlickrAccount(QObject *parent = nullptr);

    Q_INVOKABLE void obtainPinUrl();
    Q_INVOKABLE void enterPin(const QString &pin);
    Q_INVOKABLE bool isLinked();
    Q_INVOKABLE bool getUseSwipeNavigation();
    Q_INVOKABLE void setUseSwipeNavigation(const bool &useSwipeNavigation);
    Q_INVOKABLE QString getFontSize();
    Q_INVOKABLE void setFontSize(const QString &fontSize);
    Q_INVOKABLE void removeOldCacheFiles();

    FlickrApi *getFlickrApi();

signals:
    void pinRequestError(const QString &errorMessage);
    void pinRequestSuccessful(const QString &url);
    void linkingFailed(const QString &errorMessage);
    void linkingSuccessful();
    void swipeNavigationChanged();
    void fontSizeChanged(const QString &fontSize);

public slots:
    void handlePinRequestError(const QString &errorMessage);
    void handlePinRequestSuccessful(const QUrl &url);
    void handleLinkingFailed();
    void handleLinkingSucceeded();

private:
    QString encryptionKey;
    QNetworkConfigurationManager * const networkConfigurationManager;
    O1Flickr * const o1;
    QNetworkAccessManager * const manager;
    O1Requestor *requestor;
    FlickrApi *flickrApi;
    QSettings settings;

    void obtainEncryptionKey();
    void initializeEnvironment();
};

#endif // FLICKRACCOUNT_H
