#include "flickraccount.h"

#include <QDebug>
#include <QFile>
#include <QUuid>

#include "o0settingsstore.h"
#include "o1flickrglobals.h"

FlickrAccount::FlickrAccount(QObject *parent) : QObject(parent),
                                                o1(new O1Flickr(this)),
                                                networkConfigurationManager(new QNetworkConfigurationManager(this))
{
    qDebug() << "[FlickrAccount] Initializing account handler...";
    connect(o1, &O1Flickr::linkingFailed, this, &FlickrAccount::handleLinkingFailed);
    connect(o1, &O1Flickr::linkingSucceeded, this, &FlickrAccount::handleLinkingSucceeded);
    connect(o1, &O1Flickr::openBrowser, this, &FlickrAccount::handleOpenUrl);

    obtainEncryptionKey();
    initializeEnvironment();
}

void FlickrAccount::handleLinkingFailed()
{
    qDebug() << "[FlickrAccount] Linking failed! :(";
}

void FlickrAccount::handleLinkingSucceeded()
{
    qDebug() << "[FlickrAccount] Linking succeeded! :)";
}

void FlickrAccount::handleOpenUrl(const QUrl &url)
{
    qDebug() << "[FlickrAccount] Opening URL " << url.url();
    emit openUrl(url.url());
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

    O0SettingsStore *settingsStore = new O0SettingsStore(encryptionKey, this);
    o1->setStore(settingsStore);
    o1->setClientId(FLICKR_CLIENT_KEY);
    o1->setClientSecret(FLICKR_CLIENT_SECRET);

    if (!o1->linked()) {
        o1->link();
    }
}
