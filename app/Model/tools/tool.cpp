#include "tool.h"
#include "Model/framework/request.h"
#include "Model/regovar.h"
#include "Model/file/file.h"
#include "Model/analysis/filtering/filteringanalysis.h"

Tool::Tool(QObject *parent) :  QObject(parent) {}
Tool::Tool(ToolType type, QJsonObject json, QObject* parent) :  QObject(parent)
{
    mType = type;
    mKey = json["key"].toString();
    mName = json["name"].toString();
    mDescription = json["description"].toString();

    for (const QJsonValue& val: json["parameters"].toArray())
    {
        mParameters << new ToolParameter(val.toObject());
    }
}





QJsonObject Tool::toJson()
{
    QJsonObject parameters;
    for (QObject* o: mParameters)
    {
        ToolParameter* param = qobject_cast<ToolParameter*>(o);
        parameters.insert(param->key(), param->value().toJsonValue());
    }
    return parameters;
}

void Tool::clear()
{
    for (QObject* o: mParameters)
    {
        ToolParameter* param = qobject_cast<ToolParameter*>(o);
        param->clear();
    }
}

void Tool::run(int analysisId)
{
    run(analysisId, QJsonObject());
}
void Tool::run(int analysisId, QJsonObject parameter)
{
    QString cmd = (mType == Exporter) ? "export" : "report";
    Request* req = Request::post(QString("/analysis/%1/%2").arg(analysisId).arg(cmd), QJsonDocument(parameter).toJson());
    connect(req, &Request::responseReceived, [this, analysisId, req](bool success, const QJsonObject& json)
    {
        if (success)
        {
            QJsonObject data = json["data"].toObject();
            if (mType == Exporter)
            {
                File* file = regovar->filesManager()->getOrCreateFile(data["id"].toInt());
                file->loadJson(data);
                FilteringAnalysis* analysis = regovar->analysesManager()->getOrCreateFilteringAnalysis(analysisId);
                analysis->addFile(file);
            }
            else if (mType == Reporter)
            {

            }
        }
    });
}
