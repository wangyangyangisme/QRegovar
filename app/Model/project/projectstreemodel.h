#ifndef PROJECTSTREEMODEL_H
#define PROJECTSTREEMODEL_H

#include "Model/treemodel.h"

class ProjectsTreeModel : public TreeModel
{
    Q_OBJECT

    Q_PROPERTY(bool isLoading READ isLoading WRITE setIsLoading NOTIFY isLoadingUpdated)

public:

    enum JsonModelRoles
    {
        NameRole = Qt::UserRole + 1,
        CommentRole,
        DateRole,
    };


    explicit ProjectsTreeModel();
    QHash<int, QByteArray> roleNames() const override;

    void refresh();
    QVariant newProjectsTreeViewItem(int id, const QString &text);
    void setupModelData(QJsonArray data, TreeItem *parent);


    // Accessors
    inline bool isLoading() { return mIsLoading; }

    // Setters
    inline void setIsLoading(bool isLoading) { mIsLoading = isLoading; emit isLoadingUpdated(); }


Q_SIGNALS:
    void isLoadingUpdated();


private:
    bool mIsLoading;
};

#endif // PROJECTSTREEMODEL_H