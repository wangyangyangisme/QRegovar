#include "panel.h"
#include "Model/regovar.h"
#include "Model/framework/request.h"



Panel::Panel(bool rootPanel, QObject* parent) : QObject(parent)
{
    if (rootPanel)
    {
        mOrderedVersionsIds = new QStringList();
        mVersionsMap = new QHash<QString, Panel*>();
    }
}

Panel::Panel(QStringList* orderedVersions, QHash<QString, Panel*>* map, QObject* parent) : QObject(parent)
{
    mOrderedVersionsIds = orderedVersions;
    mVersionsMap = map;
}




Panel* Panel::buildPanel(QJsonObject json)
{
    // Create first panel
    Panel* panel = new Panel(true);
    if (panel->fromJson(json))
    {
        // Create all other versions of the same panel
        for (const QJsonValue& data: json["versions"].toArray())
        {
            panel->addVersion(data.toObject(), true);
        }
    }
    return panel;
}














QString Panel::addVersion(QJsonObject data, bool append)
{
    Panel* panel = new Panel(mOrderedVersionsIds, mVersionsMap);
    bool result = panel->fromJson(data);
    if (result)
    {
        mVersionsMap->insert(panel->versionId(), panel);
        if (append)
        {
            mOrderedVersionsIds->append(panel->versionId());
        }
        else
        {
            mOrderedVersionsIds->insert(0, panel->versionId());
        }
    }
    return result ? panel->versionId() : "";
}




// Load only data for the current panelversion.
bool Panel::fromJson(QJsonObject json)
{
    // json may be for Panel or Panel's version
    // a Panel contains a list of version

    if (json.contains("versions"))
    {
        // Loading Panels informations
        mOrderedVersionsIds->clear();
        mVersionsMap->clear();

        mPanelId = json["id"].toString();
        mName = json["name"].toString();
        mOwner = json["owner"].toString();
        mDescription = json["description"].toString();
        mShared = json["shared"].toBool();
        mCreateDate = QDateTime::fromString(json["creation_date"].toString(), Qt::ISODate);
        mUpdateDate = QDateTime::fromString(json["update_date"].toString(), Qt::ISODate);

        // Create all other versions of the same panel
        for (const QJsonValue& data: json["versions"].toArray())
        {
            QString versionId = addVersion(data.toObject(), true);
            // Set panelId of the new version created
            regovar->panelsManager()->getOrCreatePanel(versionId)->setPanelId(mPanelId);
        }
    }
    else
    {
        // Load version informations
        mVersionId = json["id"].toString();
        mVersion = json["version"].toString();
        mComment = json["comment"].toString();
        mCreateDate = QDateTime::fromString(json["creation_date"].toString(), Qt::ISODate);
        mUpdateDate = QDateTime::fromString(json["update_date"].toString(), Qt::ISODate);

        // Load entries
        for(const QJsonValue& entry: json["entries"].toArray())
        {
            mEntries.append(entry.toObject());
        }
    }

    mLoaded = true;
    emit dataChanged();
    return true;
}

QJsonObject Panel::toJson()
{
    QJsonObject result;
    // Simples data
    result.insert("id", mPanelId);
    result.insert("name", mName);
    result.insert("owner", mOwner);
    result.insert("description", mDescription);
    result.insert("shared", mShared);


    // Versions
    // Notice: json export is used only for Update and Create one version of the panel
    // So, we don't format json with the list of all available version as done server side
    result.insert("version", mVersion);
    QJsonArray entries;

    for(const QVariant& value: mEntries)
    {
        entries.append(value.toJsonObject());
    }
    result.insert("entries", entries);

    return result;
}





void Panel::save()
{
    if (mPanelId.isEmpty()) return;
    Request* request = Request::put(QString("/panel/%1").arg(mPanelId), QJsonDocument(toJson()).toJson());
    connect(request, &Request::responseReceived, [this, request](bool success, const QJsonObject& json)
    {
        if (success)
        {
            qDebug() << "Panel saved";
        }
        else
        {
            QJsonObject jsonError = json;
            jsonError.insert("method", Q_FUNC_INFO);
            regovar->raiseError(jsonError);
        }
        request->deleteLater();
    });
}



void Panel::load(bool forceRefresh)
{
    // Check if need refresh
    qint64 diff = mLastInternalLoad.secsTo(QDateTime::currentDateTime());
    if (!mLoaded || forceRefresh || diff > MIN_SYNC_DELAY)
    {
        mLastInternalLoad = QDateTime::currentDateTime();
        Request* req = Request::get(QString("/panel/%1").arg(mPanelId));
        connect(req, &Request::responseReceived, [this, req](bool success, const QJsonObject& json)
        {
            if (success)
            {
                fromJson(json["data"].toObject());
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
}





void Panel::addEntry(QJsonObject data)
{
    mEntries.append(data);
    emit dataChanged();
}


void Panel::reset()
{
    mName = "";
    mDescription = "";
    mOwner = "";
    mDescription = "";
    mShared = false;
    mVersion = "";
    mEntries.clear();
    emit dataChanged();
}





