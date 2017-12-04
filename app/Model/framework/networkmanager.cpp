#include "networkmanager.h"

#include "Model/regovar.h"
#include "Model/file/file.h"
#include "Model/analysis/filtering/filteringanalysis.h"
#include "Model/analysis/pipeline/pipelineanalysis.h"


NetworkManager::NetworkManager(QObject *parent) : QObject(parent)
{
    // Connections
    connect(&mWebSocket, &QWebSocket::connected, this, &NetworkManager::onWebsocketConnected);
    connect(&mWebSocket, &QWebSocket::disconnected, this, &NetworkManager::onWebsocketClosed);
    connect(&mWebSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onWebsocketError(QAbstractSocket::SocketError)));
    // connect(&mWebSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onWebsocketStateChanged(QAbstractSocket::SocketState)));
    mWebSocket.open(QUrl(mWebsocketUrl));
}







bool NetworkManager::testServerUrl(QString url)
{
    // TODO: do a request, and check if get expected result
    emit testServerUrlDone(!url.isEmpty(), url);
}


void NetworkManager::setServerUrl(QUrl url)
{
    mServerUrl = url;

    // Websocket
    mWebsocketUrl.setScheme(url.scheme() == "https" ? "wss" : "ws");
    mWebsocketUrl.setHost(url.host());
    mWebsocketUrl.setPath("/ws");
    mWebsocketUrl.setPort(url.port());

    emit serverUrlChanged();
}


void NetworkManager::setSharedServerUrl(QUrl)
{
    // TODO:
    emit sharedServerUrlChanged();
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

    // TODO: get progress and update Network task if needed
    if (mWsSamplesActionsList.indexOf(action) != -1)
    {
        regovar->samplesManager()->processPushNotification(action, data);
    }
    else if (mWsFilesActionsList.indexOf(action) != -1)
    {
        regovar->filesManager()->processPushNotification(action, data);
    }
    else if (mWsFilteringActionsList.indexOf(action) != -1)
    {
        int id = data["id"].toInt();
        FilteringAnalysis* analysis = regovar->analysesManager()->getFilteringAnalysis(id);
        if (analysis != nullptr)
        {
            analysis->processPushNotification(action, data);
        }
    }
    else if (mWsPipelinesActionsList.indexOf(action) != -1)
    {
        int id = data["id"].toInt();
        PipelineAnalysis* analysis = regovar->analysesManager()->getPipelineAnalysis(id);
        if (analysis != nullptr)
        {
            analysis->processPushNotification(action, data);
        }
    }
    else if (obj["action"].toString() != "hello")
    {
        qDebug() << "WS WARNING: Websocket Unknow message" << message;
    }
    emit websocketMessageReceived(action, data);
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

void NetworkManager::onWebsocketStateChanged(QAbstractSocket::SocketState)
{
    // qDebug() << "WS state" << state;
}







void NetworkManager::onAuthenticationRequired(QNetworkReply* request, QAuthenticator* authenticator)
{
    // Basic authentication requested by the server.
    // Try authentication using current user credentials
    if (authenticator->password() != regovar->user()->password() || authenticator->user() != regovar->user()->login())
    {
        authenticator->setUser(regovar->user()->login());
        authenticator->setPassword(regovar->user()->password());
    }
    else
    {
        request->error();
    }
}


