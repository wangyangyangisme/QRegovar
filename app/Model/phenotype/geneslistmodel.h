#ifndef GENESLISTMODEL_H
#define GENESLISTMODEL_H

#include <QtCore>
#include "Model/framework/genericproxymodel.h"

class GenesListModel: public QAbstractListModel
{
    enum Roles
    {
        Id = Qt::UserRole + 1,
        Panel,
        SearchField
    };

    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    Q_PROPERTY(GenericProxyModel* proxy READ proxy NOTIFY neverChanged)


public:
    // Constructor
    explicit GenesListModel(QObject* parent=nullptr);

    // Getters
    inline GenericProxyModel* proxy() const { return mProxy; }

    // Methods
    //! Remove all entries of the list
    Q_INVOKABLE void clear();
    //! Load phenotype list from list of json
    Q_INVOKABLE bool loadJson(QJsonArray json);
    //! Add the provided gene to the list if not already contains
    Q_INVOKABLE bool append(QString gene);
    //! Remove a gene from the list if possible
    Q_INVOKABLE bool remove(QString gene);
    //! Return entry at the requested position in the list
    Q_INVOKABLE QString getAt(int idx);
    //! oins all the string list's strings into a single string with each element separated by the given separator (which can be an empty string).
    Q_INVOKABLE QString join(QString separator);

    // QAbstractListModel methods
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;


Q_SIGNALS:
    void neverChanged();
    void countChanged();


private:
    QStringList mGenes;
    GenericProxyModel* mProxy = nullptr;
};

#endif // GENESLISTMODEL_H