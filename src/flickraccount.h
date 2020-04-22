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
    Q_INVOKABLE bool getUseOpenWith();
    Q_INVOKABLE void setUseOpenWith(const bool &useOpenWith);
    Q_INVOKABLE QString getFontSize();
    Q_INVOKABLE void setFontSize(const QString &fontSize);

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
