#ifndef DROPBOXAUTH_H
#define DROPBOXAUTH_H

#include <QObject>
#include <QOAuth2AuthorizationCodeFlow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include <QUrl>
#include <QUrlQuery>
#include <QOAuthHttpServerReplyHandler>
#include <QDesktopServices>

class Dropboxauth : public QObject
{
    Q_OBJECT
public:
    explicit Dropboxauth(QObject *parent = nullptr);
    Q_INVOKABLE void click();


private:
    QOAuth2AuthorizationCodeFlow * dropbox;
};

#endif // DROPBOXAUTH_H
