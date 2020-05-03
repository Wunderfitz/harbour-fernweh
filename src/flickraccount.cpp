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
#include "flickraccount.h"

#include <QDebug>
#include <QFile>
#include <QUuid>

#include "o0settingsstore.h"
#include "o1flickrglobals.h"

const char SETTINGS_USE_SWIPE_NAVIGATION[] = "settings/useSwipeNavigation";
const char SETTINGS_FONT_SIZE[] = "settings/fontSize";

FlickrAccount::FlickrAccount(QObject *parent) : QObject(parent),
                                                networkConfigurationManager(new QNetworkConfigurationManager(this)),
                                                o1(new O1Flickr(this)),                                                
                                                manager(new QNetworkAccessManager(this)),
                                                settings("harbour-fernweh", "settings")
{
    qDebug() << "[FlickrAccount] Initializing account handler...";
    obtainEncryptionKey();
    initializeEnvironment();
}

void FlickrAccount::obtainEncryptionKey()
{
    // We try to use the unique device ID as encryption key. If we can't determine this ID, a default key is used...
    // Unique device ID determination copied from the QtSystems module of the Qt Toolkit
    if (encryptionKey.isEmpty()) {
        QFile file(QStringLiteral("/sys/devices/virtual/dmi/id/product_uuid"));
        if (file.open(QIODevice::ReadOnly)) {
            QString id = QString::fromLocal8Bit(file.readAll().simplified().data());
            if (id.length() == 36) {
                encryptionKey = id;
            }
            file.close();
        }
    }
    if (encryptionKey.isEmpty()) {
        QFile file(QStringLiteral("/etc/machine-id"));
        if (file.open(QIODevice::ReadOnly)) {
            QString id = QString::fromLocal8Bit(file.readAll().simplified().data());
            if (id.length() == 32) {
                encryptionKey = id.insert(8,'-').insert(13,'-').insert(18,'-').insert(23,'-');
            }
            file.close();
        }
    }
    if (encryptionKey.isEmpty()) {
        QFile file(QStringLiteral("/etc/unique-id"));
        if (file.open(QIODevice::ReadOnly)) {
            QString id = QString::fromLocal8Bit(file.readAll().simplified().data());
            if (id.length() == 32) {
                encryptionKey = id.insert(8,'-').insert(13,'-').insert(18,'-').insert(23,'-');
            }
            file.close();
        }
    }
    if (encryptionKey.isEmpty()) {
        QFile file(QStringLiteral("/var/lib/dbus/machine-id"));
        if (file.open(QIODevice::ReadOnly)) {
            QString id = QString::fromLocal8Bit(file.readAll().simplified().data());
            if (id.length() == 32) {
                encryptionKey = id.insert(8,'-').insert(13,'-').insert(18,'-').insert(23,'-');
            }
            file.close();
        }
    }
    QUuid uid(encryptionKey); // make sure this can be made into a valid QUUid
    if (uid.isNull()) {
         encryptionKey = QString(FLICKR_STORE_DEFAULT_ENCRYPTION_KEY);
    }
    qDebug() << "[FlickrAccount] Using encryption key: " << encryptionKey;
}

void FlickrAccount::initializeEnvironment()
{
    qDebug() << "[FlickrAccount] Initializing environment...";
    O0SettingsStore *settingsStore = new O0SettingsStore(encryptionKey, this);
    o1->setStore(settingsStore);
    o1->setClientId(FLICKR_CLIENT_KEY);
    o1->setClientSecret(FLICKR_CLIENT_SECRET);

    connect(o1, &O1Flickr::pinRequestError, this, &FlickrAccount::handlePinRequestError);
    connect(o1, &O1Flickr::pinRequestSuccessful, this, &FlickrAccount::handlePinRequestSuccessful);
    connect(o1, &O1Flickr::linkingFailed, this, &FlickrAccount::handleLinkingFailed);
    connect(o1, &O1Flickr::linkingSucceeded, this, &FlickrAccount::handleLinkingSucceeded);

    requestor = new O1Requestor(manager, o1, this);
    flickrApi = new FlickrApi(requestor, manager, this);
}

void FlickrAccount::obtainPinUrl()
{
    if (networkConfigurationManager->isOnline()) {
        o1->obtainPinUrl();
    } else {
        emit pinRequestError("I'm sorry, your device is offline!");
    }
}

void FlickrAccount::enterPin(const QString &pin)
{
    qDebug() << "PIN entered: " + pin;
    if (networkConfigurationManager->isOnline()) {
        o1->verifyPin(pin);
    } else {
        emit linkingFailed("I'm sorry, your device is offline!");
    }
}

bool FlickrAccount::isLinked()
{
    return o1->linked();
}

bool FlickrAccount::getUseSwipeNavigation()
{
    return settings.value(SETTINGS_USE_SWIPE_NAVIGATION, true).toBool();
}

void FlickrAccount::setUseSwipeNavigation(const bool &useSwipeNavigation)
{
    settings.setValue(SETTINGS_USE_SWIPE_NAVIGATION, useSwipeNavigation);
    emit swipeNavigationChanged();
}

QString FlickrAccount::getFontSize()
{
    return settings.value(SETTINGS_FONT_SIZE, "fernweh").toString();
}

void FlickrAccount::setFontSize(const QString &fontSize)
{
    settings.setValue(SETTINGS_FONT_SIZE, fontSize);
    emit fontSizeChanged(fontSize);
}

FlickrApi *FlickrAccount::getFlickrApi()
{
    return this->flickrApi;
}

void FlickrAccount::handlePinRequestError(const QString &errorMessage)
{
    emit pinRequestError("I'm sorry, there was an error: " + errorMessage);
}

void FlickrAccount::handlePinRequestSuccessful(const QUrl &url)
{
    emit pinRequestSuccessful(url.toString());
}

void FlickrAccount::handleLinkingFailed()
{
    qDebug() << "Linking failed! :(";
    emit linkingFailed("Linking error");
}

void FlickrAccount::handleLinkingSucceeded()
{
    qDebug() << "Linking successful! :)";
    emit linkingSuccessful();
}
