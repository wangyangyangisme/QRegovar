#include <QDebug>
#include "projectstreemodel.h"
#include "projectstreeitem.h"
#include "Model/framework/request.h"



ProjectsTreeModel::ProjectsTreeModel() : TreeModel(nullptr)
{
    // With QML TreeView, the rootItem must know all column's roles to allow correct display for
    // other rows. So that's why we create columns for all existings roles.
    QHash<int, QVariant> rootData;
    QHash<int, QByteArray> roles = roleNames();
    for (const int roleId: roles.keys())
    {
        rootData.insert(roleId, QString(roles[roleId]));
    }
    mRootItem = new TreeItem(rootData);
}




void ProjectsTreeModel::refresh(QJsonObject json)
{
    beginResetModel();
    clear();
    setupModelData(json["data"].toArray(), mRootItem);
    endResetModel();
}






QHash<int, QByteArray> ProjectsTreeModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[DateRole] = "date";
    roles[CommentRole] = "comment";
    return roles;
}



QVariant ProjectsTreeModel::newProjectsTreeItem(int id, bool isAnalysis, const QString& text)
{
    ProjectsTreeItem *t = new ProjectsTreeItem(this);
    t->setText(text);
    t->setId(id);
    t->setIsAnalysis(isAnalysis);
    QVariant v;
    v.setValue(t);
    return v;
}

void ProjectsTreeModel::setupModelData(QJsonArray data, TreeItem *parent)
{
    for (const QJsonValue& json: data)
    {
        QJsonObject p = json.toObject();
        int id = p["id"].toInt();

        // Get Json data and store its into item's columns (/!\ columns order must respect enum order)
        QHash<int, QVariant> columnData;
        columnData.insert(NameRole, newProjectsTreeItem(id, false, p["name"].toString()));
        columnData.insert(CommentRole, newProjectsTreeItem(id, false, p["comment"].toString()));
        QDateTime date = QDateTime::fromString(p["update_date"].toString(), Qt::ISODate);
        columnData.insert(DateRole, newProjectsTreeItem(id, false, date.toString("yyyy-MM-dd HH:mm")));

        // Create treeview item with column's data and parent item
        TreeItem* item = new TreeItem(columnData, parent);
        parent->appendChild(item);

        // If folder, need to retrieve subitems recursively
        if (p["is_folder"].toBool())
        {
            setupModelData(p["children"].toArray(), item);
        }
        // If project, need to build subtree for analyses and subjects
        else if (p["analyses"].toArray().count() > 0)
        {
            setupModelAnalysisData(p["analyses"].toArray(), item);
        }
    }
}

void ProjectsTreeModel::setupModelAnalysisData(QJsonArray data, TreeItem *parent)
{
    for (const QJsonValue& json: data)
    {
        QJsonObject p = json.toObject();
        int id = p["id"].toInt();

        // Get Json data and store its into item's columns (/!\ columns order must respect enum order)
        QHash<int, QVariant> columnData;
        columnData.insert(NameRole, newProjectsTreeItem(id, true, p["name"].toString()));
        columnData.insert(CommentRole, newProjectsTreeItem(id, true, p["comment"].toString()));
        columnData.insert(DateRole, newProjectsTreeItem(id, true, p["update_date"].toString()));

        // Create treeview item with column's data and parent item
        TreeItem* item = new TreeItem(columnData, parent);
        parent->appendChild(item);
    }
}
