#include "request.h"
#include "Model/regovarmodel.h"
#include <QDebug>

QNetworkAccessManager* Request::mNetManager = Q_NULLPTR;
QNetworkAccessManager* Request::netManager()
{
    if (mNetManager == Q_NULLPTR)
    {
        mNetManager = new QNetworkAccessManager();
        connect ( mNetManager,
                  SIGNAL(authenticationRequired(QNetworkReply*, QAuthenticator*)),
                  RegovarModel::i(),
                  SLOT(authenticationRequired(QNetworkReply*, QAuthenticator*)));
    }

    return mNetManager;
}



Request::Request(Verb verb, const QString& query, QHttpMultiPart* data, QObject* parent) : QObject(parent)
{
    QNetworkRequest request = Request::makeRequest(query);
    mReply = Q_NULLPTR;
    switch (verb)
    {
        case Request::Get:
            mReply = Request::netManager()->get(request);
            break;
        case Request::Post:
            mReply = Request::netManager()->post(request, data);
            break;
        case Request::Put:
            mReply = Request::netManager()->put(request, data);
            break;
        case Request::Del:
            mReply = Request::netManager()->deleteResource(request);
            break;

        default:
            qCritical() << Q_FUNC_INFO << "Unknow query verb ... \"" << verb << "\" : \"" << query << "\"";
            break;
    }
    if (mReply != Q_NULLPTR)
    {
        mLoading = true;
        connect(mReply, SIGNAL(finished()), this,SLOT(received()));
    }
}






Request* Request::get(const QString query)
{
    return new Request(Get, query);
}

Request* Request::put(const QString query, QHttpMultiPart* data)
{
    return new Request(Put, query, data);
}

Request* Request::post(const QString query, QHttpMultiPart* data)
{
    return new Request(Post, query, data);
}

Request* Request::del(const QString query)
{
    return new Request(Del, query);
}

QNetworkRequest Request::makeRequest(const QString& resource)
{
    QUrl url(RegovarModel::i()->serverUrl());
    url.setPath(resource);
    qDebug() << Q_FUNC_INFO << url;

    QNetworkRequest request(url);
    return request;
}




const QJsonObject &Request::json() const
{
    return mJson;
}

void Request::received()
{
    mLoading = false;
    QNetworkReply * reply = qobject_cast<QNetworkReply*>(sender());

    if (reply && reply->isFinished())
    {
        mReplyError = reply->error();
        if (mReplyError == QNetworkReply::NoError)
        {
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            mJson = doc.object();
            mSuccess = mJson["success"].toBool();
            emit responseReceived(mSuccess, mJson);
        }
        else
        {
            qDebug() << "ERROR" << Q_FUNC_INFO << mReplyError;
            mSuccess = false;
            mJson = QJsonObject();
            emit responseReceived(mSuccess, mJson);
        }
    }
    else
    {
        qWarning() << Q_FUNC_INFO << "Request ends with no answer ? ";
    }
}