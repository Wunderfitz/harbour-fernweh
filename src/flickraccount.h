#ifndef FLICKRACCOUNT_H
#define FLICKRACCOUNT_H

#include <QObject>
#include <QNetworkConfigurationManager>
#include <QNetworkAccessManager>

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

    FlickrApi *getFlickrApi();

signals:
    void pinRequestError(const QString &errorMessage);
    void pinRequestSuccessful(const QString &url);
    void linkingFailed(const QString &errorMessage);
    void linkingSuccessful();

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

    void obtainEncryptionKey();
    void initializeEnvironment();
};

#endif // FLICKRACCOUNT_H
