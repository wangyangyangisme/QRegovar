#include "usersmanager.h"
#include "Model/framework/request.h"
#include "Model/regovar.h"

UsersManager::UsersManager(QObject *parent) : QObject(parent)
{
    mUsersList = new UsersListModel(this);
    mUser = new User();
    mNewUser = new User();
}



void UsersManager::loadJson(QJsonArray json)
{
    for (const QJsonValue& val: json)
    {
        QJsonObject data = val.toObject();
        int id = data["id"].toInt();
        User* user = getOrCreateUser(id);
        user->loadJson(data);
        mUsersList->append(user);
    }
}





User* UsersManager::getOrCreateUser(int userId)
{
    if (mUsers.contains(userId))
    {
        return mUsers[userId];
    }
    // else
    User* newUser= new User(userId, this);
    mUsers.insert(userId, newUser);
    return newUser;
}


void UsersManager::processPushNotification(QString action, QJsonObject data)
{
//    // Retrieve realtime progress data
//    QString status = data["status"].toString();
//    double progressValue = 0.0;
//    if (action == "import_vcf_processing" || action == "import_vcf_start")
//    {
//        progressValue = data["progress"].toDouble();
//    }
//    else if (action == "import_vcf_end")
//    {
//        progressValue = 1.0;
//        status = "ready";
//    }

//    // Update sample status
//    for (const QJsonValue& json: data["samples"].toArray())
//    {
//        QJsonObject obj = json.toObject();
//        int sid = obj["id"].toInt();

//        Sample* sample = getOrCreateEvent(sid);
//        sample->setStatus(status);
//        sample->setLoadingProgress(progressValue);
//        sample->refreshUIAttributes();
//    }

//    // Notify view when new sample import start (import wizard)
//    if (action == "import_vcf_start")
//    {
//        QList<int> ids;
//        for (QJsonValue sample: data["samples"].toArray())
//        {
//            QJsonObject sampleData = sample.toObject();
//            ids << sampleData["id"].toInt();
//        }
//        emit sampleImportStart(data["file_id"].toString().toInt(), ids);
//    }
}



void UsersManager::login(QString login, QString password)
{

    // Store login and password as it may be ask later if network authentication problem (see Regovar::onAuthenticationRequired)
    mUser->setLogin(login);
    mUser->setPassword(password);

    QJsonObject body;
    body.insert("login", login);
    body.insert("password", password);

    Request* req = Request::post("/user/login", QJsonDocument(body).toJson());
    connect(req, &Request::responseReceived, [this, req](bool success, const QJsonObject& json)
    {
        if (success)
        {
            QJsonObject data = json["data"].toObject();
            setUser(getOrCreateUser(data["id"].toInt()));
            mUser->loadJson(data);
            emit displayLoginScreen(false);
            regovar->loadWelcomData();
            regovar->settings()->setKeepMeLogged(mKeepMeLogged);
            if (mKeepMeLogged)
            {
                regovar->settings()->setSessionUserId(mUser->id());
                for (QNetworkCookie cookie: Request::netManager()->cookieJar()->cookiesForUrl(regovar->settings()->serverUrl()))
                {
                    if (cookie.name().toStdString() == "regovar_session")
                    {
                        regovar->settings()->setSessionCookie(cookie);
                    }
                }
            }
        }
        else
        {
            emit loginFailed();
        }
        req->deleteLater();
    });
}


void UsersManager::logout()
{
    if (mUser->isValid())
    {
        Request* test = Request::get("/user/logout");
        connect(test, &Request::responseReceived, [this](bool, const QJsonObject&)
        {
            mUser->clear();
            qDebug() << Q_FUNC_INFO << "You are disconnected !";
            emit logoutSuccess();
            emit displayLoginScreen(true);
        });
    }
}


void UsersManager::switchLoginScreen(bool state)
{
    emit displayLoginScreen(state);
}


