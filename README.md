# Qt-Dropbox-oauth2

Sample application to showcase how to use Qt C++ to authenticate Dropbox apps. I will keep the repo updated as Qt and Dropbox introduces more changes.

Feel free to improve the code and post your issues

## Files

#### Qt-Dropbox-oauth2.pro

Remember to add `networkauth` in the list of Qt module dependencies

```sh
QT += networkauth
```

#### dropboxauth.h & dropboxauth.cpp

Please read the comments in the files. I have tried to explain all the details of the code.
Remember to edit the `APP_KEY`, `APP_SECRET` & `port_number` (in my case, it is 5476) in dropboxauth.cpp

```sh
this->dropbox->setClientIdentifier("APP_KEY");
this->dropbox->setClientIdentifierSharedKey("APP_SECRET");
auto replyHandler = new QOAuthHttpServerReplyHandler(5476, this);
```

#### main.cpp

Initialized the dropboxauth object

#### main.qml

Contains the button which calls the `grant()` method of Qt and prompts Dropbox auth on the browser

#### Others

-   [Dropbox developer console screenshot](DropboxConsoleScreenshot.png): Remember to put http://127.0.0.1:your_port instead of http://localhost:your_port else this will result in `redirect_uri mismatch error`.

Remember, the Redirect URL has to be exactly same as mentioned in the Dropbox developer console
