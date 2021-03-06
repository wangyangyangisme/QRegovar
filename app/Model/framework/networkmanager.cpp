#include "networkmanager.h"

#include "Model/regovar.h"


NetworkManager::NetworkManager(QObject *parent) : QObject(parent)
{
    // Connections
    connect(&mWebSocket, &QWebSocket::connected, this, &NetworkManager::onWebsocketConnected);
    connect(&mWebSocket, &QWebSocket::disconnected, this, &NetworkManager::onWebsocketClosed);
    connect(&mWebSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onWebsocketError(QAbstractSocket::SocketError)));
    // connect(&mWebSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onWebsocketStateChanged(QAbstractSocket::SocketState)));
}







bool NetworkManager::testServerUrl(QString serverUrl, QString sharedUrl)
{
    // TODO: do a request, and check if get expected result
    bool test1 = !serverUrl.isEmpty();
    bool test2 = !sharedUrl.isEmpty();
    if (test1)
    {
        mServerUrl = QUrl(serverUrl);
        regovar->settings()->setServerUrl(QUrl(serverUrl));
    }
    if (test1)
    {
        mSharedUrl = QUrl(sharedUrl);
        regovar->settings()->setSharedUrl(QUrl(sharedUrl));
    }
    if(test1 || test2)
    {
        regovar->settings()->save();
    }


    regovar->loadConfigData();

    emit testServerUrlDone(test1, mServerUrl.toString(), mSharedUrl.toString());
    return true;
}


void NetworkManager::setServerUrl(QUrl url)
{
    mServerUrl = url;

    // Websocket
    mWebsocketUrl.setScheme(url.scheme() == "https" ? "wss" : "ws");
    mWebsocketUrl.setHost(url.host());
    mWebsocketUrl.setPath("/ws");
    mWebsocketUrl.setPort(url.port());

    // Disconnect to former url and reconnect to the new
    mWebSocket.close();
    mWebSocket.open(QUrl(mWebsocketUrl));

    emit serverUrlChanged();
}

















void NetworkManager::onWebsocketConnected()
{
    setStatus(ready);
    connect(&mWebSocket, &QWebSocket::textMessageReceived, this, &NetworkManager::onWebsocketReceived);
    mWebSocket.sendTextMessage(QStringLiteral("{ \"action\" : \"hello\"}"));
}

void NetworkManager::onWebsocketReceived(QString message)
{

    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject obj = doc.object();
    QString action = obj["action"].toString();
    QJsonObject data = obj["data"].toObject();

    if (action != "hello")
    {
        regovar->serverNotificationReceived(action, data);
        emit websocketMessageReceived(action, data);
    }
    else
    {
        //qDebug() << "WS WARNING: Websocket Unknow message" << message;
    }
}

void NetworkManager::onWebsocketClosed()
{
    disconnect(&mWebSocket, &QWebSocket::textMessageReceived, 0, 0);
    mWebSocket.open(QUrl(mWebsocketUrl));
}

void NetworkManager::onWebsocketError(QAbstractSocket::SocketError err)
{
    if (err != QAbstractSocket::RemoteHostClosedError)
    {
        qDebug() << "WS ERROR : " << err;
        setStatus(err == QAbstractSocket::ConnectionRefusedError ? unreachable : error);
    }
}

void NetworkManager::onWebsocketStateChanged(QAbstractSocket::SocketState state)
{
    qDebug() << "WS state" << state;
}







void NetworkManager::onAuthenticationRequired(QNetworkReply* request, QAuthenticator* authenticator)
{
    // Basic authentication requested by the server.
    // Try authentication using current user credentials
    User* user = regovar->usersManager()->user();
    if (user!= nullptr && user->isValid())
    {
        if (authenticator->password() != user->password() || authenticator->user() != user->login())
        {
            authenticator->setUser(user->login());
            authenticator->setPassword(user->password());
        }
    }
    else
    {
        request->error();
    }
}



