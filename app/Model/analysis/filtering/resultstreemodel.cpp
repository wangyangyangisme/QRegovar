#include <QDebug>
#include <QtNetwork>
#include "resultstreemodel.h"
#include "resultstreeitem.h"
#include "Model/framework/request.h"
#include "Model/regovar.h"



ResultsTreeModel::ResultsTreeModel(FilteringAnalysis* parent) : TreeModel(parent)
{
    mFilteringAnalysis = parent;
    QHash<int, QVariant> rootData;
    mRootItem = new TreeItem(rootData);

    mAnalysisId = -1;
    mPagination = 1000; // TODO : load from settings
}


void ResultsTreeModel::initAnalysisData(int analysisId)
{
    qDebug() << "Init results tree model of filtering analysis" << analysisId << ":";
    mAnalysisId = analysisId;
    mRoles = roleNames();
    mAutoLoadingNext = false;

    // With QML TreeView, the rootItem must know all column's roles to allow correct display for
    // other rows. So that's why we create columns for all existings roles.
    QHash<int, QVariant> rootData;
    for (const int roleId: mRoles.keys())
    {
        rootData.insert(roleId, QString(mRoles[roleId]));
    }
    mRootItem = new TreeItem(rootData);

    beginResetModel();
    clear();
    setTotal(0);
    setLoaded(0);
    endResetModel();
    setIsLoading(false);
}



QHash<int, QByteArray> ResultsTreeModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    int roleId = Qt::UserRole + 1;
    roles[roleId] = "id";
    ++roleId;
    roles[roleId] = "is_selected";
    ++roleId;
    // Build role from annotations all annotations available list
    for (const QString& uid: mFilteringAnalysis->annotationsMap().keys())
    {
        roles[roleId] = uid.toUtf8();
        ++roleId;
    }
    return roles;
}


bool ResultsTreeModel::canFetchMore(const QModelIndex& parent) const
{
    // Don't use fetch mechanism to populate treeview roots items
    // Lazy loading managed by ourself (see how loadNext() is called by QML)
    if (!parent.isValid() || parent == QModelIndex())
        return false;

    // for other item, check if variant result pointed by the index have trx
    TreeItem* item = getItem(parent);
    if (item)
        return item->virtualChildCount() > 0 && item->children().count() != item->virtualChildCount() ;
    return false;
}


void ResultsTreeModel::fetchMore(const QModelIndex& parent)
{
    // Check if model is ready
    if (mAnalysisId == -1) return;

    // Don't use fetch mechanism to populate treeview roots items
    // Lazy loading managed by ourself (see how loadNext() is called by QML)
    if (parent == QModelIndex())
        return;



    QJsonObject body;
    body.insert("fields", QJsonArray::fromStringList(mFilteringAnalysis->fields()));
    body.insert("order", QJsonArray::fromStringList(mFilteringAnalysis->order()));

    TreeItem* item = getItem(parent);
    QString itemId = item->data(Qt::UserRole + 1).toString();
    Request* request = Request::post(QString("/analysis/%1/filtering/%2").arg(mAnalysisId).arg(itemId), QJsonDocument(body).toJson());
    connect(request, &Request::responseReceived, [this, request, parent, item](bool success, const QJsonObject& json)
    {
        if (success)
        {
            int count = item->virtualChildCount();
            beginInsertRows(parent, 0, count-1);

            QJsonObject data = json["data"].toObject();
            setLoaded(0);
            setupModelData(data["results"].toArray(), item);
            endInsertRows();
            qDebug() << Q_FUNC_INFO << "Results TreeViewModel insert trx.";
        }
        else
        {
            QJsonObject jsonError = json;
            jsonError.insert("method", Q_FUNC_INFO);
            regovar->raiseError(jsonError);
        }
        setIsLoading(false);
        request->deleteLater();
    });
}

bool ResultsTreeModel::hasChildren(const QModelIndex &parent) const
{
    TreeItem* item = getItem(parent);
    if(item)
        return item->virtualChildCount() > 0;
    return false;
}







//! Reset the Treemodel with data for the current filter set in the FilteringAnalysis
void ResultsTreeModel::applyFilter(QJsonArray filter)
{
    // Check if model is ready
    if (mAnalysisId == -1) return;

    setIsLoading(true);
    mAutoLoadingNext = false; // abord previous "load all" if still in progress

    // Save last applied filter
    regovar->analysesManager()->getFilteringAnalysis(mAnalysisId)->setFilterJson(filter);

    QJsonObject body;
    body.insert("filter", filter);
    body.insert("fields", QJsonArray::fromStringList(mFilteringAnalysis->fields()));
    body.insert("order", QJsonArray::fromStringList(mFilteringAnalysis->order()));

    Request* request = Request::post(QString("/analysis/%1/filtering").arg(mAnalysisId), QJsonDocument(body).toJson());
    connect(request, &Request::responseReceived, [this, request](bool success, const QJsonObject& json)
    {
        if (success)
        {
            beginResetModel();
            clear();

            QJsonObject data = json["data"].toObject();
            setLoaded(0);
            setTotal(data["wt_total_variants"].toInt()); // , data["wt_total_results"].toInt()
            setupModelData(data["results"].toArray(), mRootItem);
            endResetModel();
            qDebug() << Q_FUNC_INFO << "Results TreeViewModel reset." << mLoaded << "results loaded";
        }
        else
        {
            QJsonObject jsonError = json;
            jsonError.insert("method", Q_FUNC_INFO);
            regovar->raiseError(jsonError);
        }
        setIsLoading(false);
        request->deleteLater();
    });
}


//! To use when new columns have been added, to add info in the model without reseting it
void ResultsTreeModel::reload()
{

}

//! Load next result according to the mResultsPagination value (default is 100)
void ResultsTreeModel::loadNext()
{
    qDebug() << "RESULTS TREEVIEW LOADNEXT !!!!!!!!!!!!!";
    // Find how many entries to load
    int remainder = mTotal - mLoaded;
    int itemsToFetch = qMin(mPagination, remainder);

    // Notify view/model that we are loading new data
    beginInsertRows(QModelIndex(), mLoaded, mLoaded+itemsToFetch-1);
    setIsLoading(true);

    // Request the server to retrieve new entries
    QJsonObject body;
    // No need to resend the filter as it is the same, just fields and pagination information are needed
    body.insert("fields", QJsonArray::fromStringList(mFilteringAnalysis->fields()));
    body.insert("limit", QJsonValue::fromVariant(QVariant(mPagination)));
    body.insert("offset", QJsonValue::fromVariant(QVariant(mLoaded)));

    Request* request = Request::post(QString("/analysis/%1/filtering").arg(mAnalysisId), QJsonDocument(body).toJson());
    connect(request, &Request::responseReceived, [this, request](bool success, const QJsonObject& json)
    {
        if (success)
        {
            QJsonObject data = json["data"].toObject();
            setupModelData(data["results"].toArray(), mRootItem);

            qDebug() << Q_FUNC_INFO << "Results TreeViewModel load next." << mLoaded << "results loaded";
        }
        else
        {
            QJsonObject jsonError = json;
            jsonError.insert("method", Q_FUNC_INFO);
            regovar->raiseError(jsonError);
        }

        // Notify view/model that update is finished
        endInsertRows();
        setIsLoading(false);

        request->deleteLater();
    });
}


//! Load next result according to the mResultsPagination value (default is 100)
void ResultsTreeModel::loadAll()
{
    if (mAutoLoadingNext) return; // load all already in progress

    qDebug() << "RESULTS TREEVIEW LOADALL !!!!!!!!!!!!!";
    mAutoLoadingNext = true;
    loadNext();
}







//! Create treeview item with provided data, according to the type of the annotation
TreeItem* ResultsTreeModel::newResultsTreeViewItem(const QJsonObject& rowData)
{
    QString rowId = rowData["id"].toString();
    bool isSelected = rowData["is_selected"].toBool();
    QHash<int, QVariant> columnData;



    // add columns info to the item
    columnData.insert(mRoles.key("id"), QVariant(rowId));
    columnData.insert(mRoles.key("is_selected"), QVariant(isSelected));
    for (const QString& fuid: mFilteringAnalysis->fields())
    {
        columnData.insert(mRoles.key(fuid.toUtf8()), rowData[fuid].toVariant());
    }

    ResultsTreeItem* result = new ResultsTreeItem(mFilteringAnalysis);
    result->setUid(rowId);
    result->setIsSelected(isSelected);
    result->setData(columnData);

    if (rowId.contains("_"))
    {
        // Trx row
        result->setVirtualChildCount(0);
    }
    else
    {
        result->setVirtualChildCount(rowData["trx_count"].toInt());
    }

    return result;
}


void ResultsTreeModel::setupModelData(QJsonArray data, TreeItem *parent)
{
    int loaded = 0;
    for (const QJsonValue& json: data)
    {
        // Create treeview item with column's data and parent item
        TreeItem* item = newResultsTreeViewItem(json.toObject());
        parent->appendChild(item);
        ++loaded;
    }
    setLoaded(mLoaded + loaded);
    qDebug() << "Result Model Ready :" << parent->childCount() << "items loaded";
}
