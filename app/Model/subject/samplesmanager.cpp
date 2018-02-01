#include "samplesmanager.h"
#include "Model/regovar.h"
#include "Model/framework/request.h"

SamplesManager::SamplesManager(QObject *parent) : QObject(parent)
{
}
SamplesManager::SamplesManager(int refId, QObject *parent) : QObject(parent)
{
    setReferenceId(refId);
}


Sample* SamplesManager::getOrCreate(int sampleId)
{
    if (mSamples.contains(sampleId))
    {
        return mSamples[sampleId];
    }
    // else
    Sample* newSample = new Sample(sampleId, this);
    mSamples.insert(sampleId, newSample);
    mSamplesList.append(newSample); // TODO: need to insert respecting sort order ?
    return newSample;
}






void SamplesManager::setReferenceId(int refId)
{
    if (refId == mRefId) return;
    mRefId = refId;

    Request* req = Request::get(QString("/sample/browse/%1").arg(refId));
    connect(req, &Request::responseReceived, [this, req](bool success, const QJsonObject& json)
    {
        if (success)
        {
            mSamplesList.clear();
            for (const QJsonValue& sbjData: json["data"].toArray())
            {
                QJsonObject subject = sbjData.toObject();
                // TODO subject info
                for (const QJsonValue& splData: subject["samples"].toArray())
                {
                    QJsonObject sampleData = splData.toObject();
                    Sample* sample = regovar->samplesManager()->getOrCreate(sampleData["id"].toInt());
                    sample->fromJson(sampleData);
                    if (!mSamplesList.contains(sample)) mSamplesList.append(sample);
                }
            }
            emit referencialIdChanged();
            emit samplesListChanged();
        }
        else
        {
            QJsonObject jsonError = json;
            jsonError.insert("method", Q_FUNC_INFO);
            regovar->raiseError(jsonError);
        }
        req->deleteLater();
    });
}



void SamplesManager::processPushNotification(QString action, QJsonObject data)
{
    // Retrieve realtime progress data
    QString status = data["status"].toString();
    double progressValue = 0.0;
    if (action == "import_vcf_processing" || action == "import_vcf_start")
    {
        progressValue = data["progress"].toDouble();
    }
    else if (action == "import_vcf_end")
    {
        progressValue = 1.0;
        status = "ready";
    }

    // Update sample status
    for (const QJsonValue& json: data["samples"].toArray())
    {
        QJsonObject obj = json.toObject();
        int sid = obj["id"].toInt();

        Sample* sample = getOrCreate(sid);
        sample->setStatus(status);
        sample->setLoadingProgress(progressValue);
        sample->refreshUIAttributes();
    }

    // Notify view when new sample import start (import wizard)
    if (action == "import_vcf_start")
    {
        QList<int> ids;
        for (QJsonValue sample: data["samples"].toArray())
        {
            QJsonObject sampleData = sample.toObject();
            ids << sampleData["id"].toInt();
        }
        emit sampleImportStart(data["file_id"].toString().toInt(), ids);
    }
}
