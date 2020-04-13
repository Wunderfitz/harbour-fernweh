#ifndef FLICKRACCOUNT_H
#define FLICKRACCOUNT_H

#include <QObject>
#include <QNetworkConfigurationManager>

#include "o1flickr.h"

class FlickrAccount : public QObject
{
    Q_OBJECT
public:
    explicit FlickrAccount(QObject *parent = nullptr);

signals:
    void openUrl(const QString &url);

public slots:
    void handleLinkingFailed();
    void handleLinkingSucceeded();
    void handleOpenUrl(const QUrl &url);

private:
    QString encryptionKey;
    O1Flickr *o1;
    QNetworkConfigurationManager const *networkConfigurationManager;

    void obtainEncryptionKey();
    void initializeEnvironment();
};

#endif // FLICKRACCOUNT_H
