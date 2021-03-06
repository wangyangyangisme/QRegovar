#include "sample.h"
#include "Model/regovar.h"
#include "Model/framework/request.h"



Sample::Sample(QObject *parent) : RegovarResource(parent)
{
    connect(this, &Sample::dataChanged, this, &Sample::updateSearchField);
}

Sample::Sample(int id, QObject* parent) : Sample(parent)
{
    mId = id;
}
Sample::Sample(QJsonObject json, QObject* parent) : Sample(parent)
{
    loadJson(json);
}



void Sample::updateSearchField()
{
    mSearchField = mName + " " + mNickname + " " + mComment + " " + statusToLabel(mStatus, mLoadingProgress);
    if (mSubject != nullptr)
    {
        mSearchField +=  " " + mSubject->identifier() + " " + mSubject->firstname() + " " + mSubject->lastname() + " " + mSex;
    }
    if (mSource != nullptr)
    {
        mSearchField += " " + mSource->name();
    }
}
void Sample::propagateDataChanged()
{
    // When the source file of the sample emit a datachange, the sample also need to
    // notify its view to refresh
    File* file = (File*) sender();
    if (file!= nullptr && file == mSource)
    {
        emit dataChanged();
    }
}


void Sample::setStatus(const QString& status)
{
    QString s = status;
    if (status.isEmpty())
    {
        s = "empty";
    }

    auto meta = QMetaEnum::fromType<SampleStatus>();
    setStatus(static_cast<SampleStatus>(meta.keyToValue(s.toStdString().c_str()))); // T_T .... tout ça pour ça ....
}

bool Sample::loadJson(const QJsonObject& json, bool)
{
    // load basic data from json
    mId = json["id"].toInt();
    setName(json["name"].toString());
    setNickname(""); // Todo
    setIsMosaic(json["is_mosaic"].toBool());
    setComment(json["comment"].toString());
    mStats = json["stats"].toObject();
    mUpdateDate = QDateTime::fromString(json["update_date"].toString(), Qt::ISODate);
    mCreateDate = QDateTime::fromString(json["create_date"].toString(), Qt::ISODate);
    mLoadingProgress = json["loading_progress"].toDouble();

    mReference = regovar->referenceFromId(json["reference_id"].toInt());

    mDefaultAnnotationsDbUid.clear();
    for (const QJsonValue& field: json["default_dbuid"].toArray())
    {
        mDefaultAnnotationsDbUid << field.toString();
    }

    setStatus(json["status"].toString());


    File* source = regovar->filesManager()->getOrCreateFile(json["file_id"].toInt());
    setSource(source);
    if (json.contains("file"))
    {
        source->loadJson(json["file"].toObject());
    }
    setSourceUI(source->filenameUI());

    // Retrieve subject
    int subjectId = json["subject_id"].toInt();
    if (subjectId > 0)
    {
        mSubject = regovar->subjectsManager()->getOrCreateSubject(subjectId);
    }

    refreshUIAttributes();

    mLoaded = true;
    emit dataChanged();
    return true;
}


void Sample::refreshUIAttributes()
{
    QJsonObject nameInfo;
    nameInfo.insert("name", mName);
    nameInfo.insert("nickname", mNickname);
    setNameUI(QVariant::fromValue(nameInfo));

    QJsonObject statusInfo;
    statusInfo.insert("status", mStatus);
    statusInfo.insert("label", statusToLabel(mStatus, mLoadingProgress));
    statusInfo.insert("progress", mLoadingProgress);
    setStatusUI(QVariant::fromValue(statusInfo));

    emit dataChanged();
}


QJsonObject Sample::toJson()
{
    QJsonObject result;
    // Simples data
    result.insert("id", mId);
    result.insert("name", mName);
    result.insert("is_mosaic", mIsMosaic);
    result.insert("comment", mComment);
    if (mSubject != nullptr && mSubject->id() != -1)
    {
        result.insert("subject_id", mSubject->id());
    }
    else
    {
        result.insert("subject_id", QJsonValue::Null);
    }

    return result;
}



void Sample::save()
{
    if (mId == -1) return;

    Request* request = Request::put(QString("/sample/%1").arg(mId), QJsonDocument(toJson()).toJson());
    connect(request, &Request::responseReceived, [this, request](bool success, const QJsonObject& json)
    {
        if (success)
        {
            qDebug() << "Sample saved";
        }
        else
        {
            regovar->manageServerError(json, Q_FUNC_INFO);
        }
        request->deleteLater();
    });
}



void Sample::load(bool forceRefresh)
{
    // Check if need refresh
    qint64 diff = mLastInternalLoad.secsTo(QDateTime::currentDateTime());
    if (!mLoaded || forceRefresh || diff > MIN_SYNC_DELAY)
    {
        mLastInternalLoad = QDateTime::currentDateTime();
        Request* req = Request::get(QString("/sample/%1").arg(mId));
        connect(req, &Request::responseReceived, [this, req](bool success, const QJsonObject& json)
        {
            if (success)
            {
                loadJson(json["data"].toObject());
            }
            else
            {
                regovar->manageServerError(json, Q_FUNC_INFO);
            }
            req->deleteLater();
        });
    }
}



QString Sample::statusToLabel(SampleStatus status, double progress)
{

    if(status == loading)
    {
        return tr("Loading") + QString(" (%1%)").arg(QString::number( progress*100, 'f', 2 ));
    }
    if(status == ready) return tr("Ready");
    if(status == empty) return tr("Waiting for loading...");
    return tr("Error");
}
