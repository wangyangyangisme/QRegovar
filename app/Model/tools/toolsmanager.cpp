#include "toolsmanager.h"
#include "Model/framework/request.h"
#include "Model/regovar.h"
#include "tool.h"



ToolsManager::ToolsManager(QObject* parent) : QObject(parent)
{
    Request* req = Request::get(QString("/config/tools"));
    connect(req, &Request::responseReceived, [this, req](bool success, const QJsonObject& json)
    {
        if (success)
        {
            QJsonObject data = json["data"].toObject();
            // Get custom export tools deployed
            foreach (QJsonValue val, data["exporters"].toArray())
            {
                mExporters.append(new Tool(val.toObject()));
            }
            // TODO: Get custom report tools deployed
        }
    });
}
