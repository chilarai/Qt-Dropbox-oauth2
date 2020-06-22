#include "dropboxauth.h"

Dropboxauth::Dropboxauth(QObject *parent) : QObject(parent),
    m_networkAccessManager(new QNetworkAccessManager(this))
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
    this->dropbox->setAccessTokenUrl(QUrl("https://api.dropboxapi.com/oauth2/token"));
    this->dropbox->setClientIdentifier("APP_KEY");
    this->dropbox->setClientIdentifierSharedKey("APP_SECRET");


    // In my case, I have hardcoded 5476
    // This is set in Redirect URI in Dropbox Developers Console of the app

    auto replyHandler = new QOAuthHttpServerReplyHandler(5476, this);
    this->dropbox->setReplyHandler(replyHandler);


    // If access granted, pull up file list

    connect(this->dropbox, &QOAuth2AuthorizationCodeFlow::granted, [=](){
        qDebug() << __FUNCTION__ << __LINE__ << "Access Granted!";

        QJsonObject obj;
        obj.insert("limit", 100);

        QJsonDocument doc(obj);
        QString strJson(doc.toJson(QJsonDocument::Compact));

        QNetworkRequest m_networkRequest;
        m_networkRequest.setUrl(QUrl("https://api.dropboxapi.com/2/file_requests/list_v2"));

        m_networkRequest.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
        m_networkRequest.setRawHeader("Authorization", "Bearer " + this->dropbox->token().toUtf8());

        auto reply = m_networkAccessManager->post(m_networkRequest, strJson.toUtf8());

        connect(reply, &QNetworkReply::finished, [=](){
            qDebug() << "REQUEST FINISHED. Error? " << (reply->error() != QNetworkReply::NoError);
            qDebug() << reply->readAll();
        });
    });
}

// Call this function to prompt authentication
// and receive data from Dropbox

void Dropboxauth::click()
{
    this->dropbox->grant();
}
