#ifndef ANNOTATIONSTREEMODEL_H
#define ANNOTATIONSTREEMODEL_H

#include "Model/framework/treemodel.h"
#include "annotation.h"
#include "fieldcolumninfos.h"

class AnnotationsTreeModel : public TreeModel
{
    Q_OBJECT
    Q_PROPERTY(bool isLoading READ isLoading WRITE setIsLoading NOTIFY isLoadingChanged)

public:

    enum AnnotationsModelRoles
    {
        IdRole = Qt::UserRole + 1,
        CheckedRole,
        NameRole,
        VersionRole,
        DescriptionRole,
    };

    // Constructor
    explicit AnnotationsTreeModel(QObject* parent=nullptr);

    // Getters
    inline bool isLoading() { return mIsLoading; }

    // Setters
    inline void setIsLoading(bool isLoading) { mIsLoading = isLoading; emit isLoadingChanged(); }

    // Methods
    Q_INVOKABLE Annotation* getAnnotation(QString uid) ;
    Q_INVOKABLE Annotation* getAnnotation(const QModelIndex &index);

    QHash<int, QByteArray> roleNames() const override;
    inline QHash<QString, Annotation*>* annotations() { return &mAnnotations;}
    inline void addAnnotation(QString uid, Annotation* annotation) {mAnnotations.insert(uid, annotation); }
    bool fromJson(QJsonObject data, QStringList dbUids);
    void addEntry(QString dbName, QString dbVersion, QString dbDescription, bool isDbSelected, FieldColumnInfos* data);
    void setupModelData(QJsonArray data, TreeItem *parent, QStringList dbUids);

Q_SIGNALS:
    void isLoadingChanged();

private:
    bool mIsLoading = false;
    int mRefId = -1;
    QString mRefName;
    QHash<QString, Annotation*> mAnnotations;
};

#endif // ANNOTATIONSTREEMODEL_H
