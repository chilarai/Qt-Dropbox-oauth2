#include "dropboxauth.h"

Dropboxauth::Dropboxauth(QObject *parent) : QObject(parent),
    m_networkAccessManager(new QNetworkAccessManager(this)),
    m_networkReply(nullptr)
{

    this->dropbox = new QOAuth2AuthorizationCodeFlow(this);

    // Set Scope or Permissions required from Dropbox

    // This parameter allows your user to authorize a subset of the scopes selected in the App Console.
    // Multiple scopes are separated by a space. If this parameter is omitted,
    // the authorization page will request all scopes selected on the Permissions tab.


    // Uncomment the line below to set scope
    // this->dropbox->setScope("");


    connect(this->dropbox, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, [=](QUrl url) {
        QUrlQuery query(url);

        query.addQueryItem("force_reapprove", "true"); // Param required to get data everytime
        query.addQueryItem("token_access_type", "offline"); // Needed for Refresh Token (as AccessToken expires shortly)
        query.addQueryItem("response_type", "code");

        url.setQuery(query);
        QDesktopServices::openUrl(url);
    });

    // Here the parameters from Dropbox JSON are filled up
    // Attached screenshot of Dropbox Console

    this->dropbox->setAuthorizationUrl(QUrl("https://www.dropbox.com/oauth2/authorize"));
    this->dropbox->setClientIdentifier("63tucv4hmxxr4rf");
    this->dropbox->setAccessTokenUrl(QUrl("https://api.dropboxapi.com/oauth2/token"));
    this->dropbox->setClientIdentifierSharedKey("u29h3ibejo1vqav");


    // In my case, I have hardcoded 5476
    // This is set in Redirect URI in Dropbox Developers Console of the app

    auto replyHandler = new QOAuthHttpServerReplyHandler(5476, this);
    this->dropbox->setReplyHandler(replyHandler);


    connect(this->dropbox, &QOAuth2AuthorizationCodeFlow::granted, [=](){
        qDebug() << __FUNCTION__ << __LINE__ << "Access Granted!";

        QVariantMap params;
        params.insert("limit", 100);

        auto m_networkReply = this->dropbox->post(QUrl("https://api.dropboxapi.com/2/file_requests/list_v2"), params);

        connect(m_networkReply, &QNetworkReply::finished, [=](){
            qDebug() << "REQUEST FINISHED. Error? " << (m_networkReply->error() != QNetworkReply::NoError);
            qDebug() << m_networkReply->readAll();
        });
    });
}

// Call this function to prompt authentication
// and receive data from Dropbox

void Dropboxauth::click()
{
    this->dropbox->grant();
}
