#ifndef PHENOTYPESMANAGER_H
#define PHENOTYPESMANAGER_H

#include <QtCore>
#include "phenotype.h"
#include "hpodatalistmodel.h"

class PhenotypesManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(HpoDataListModel* searchResults READ searchResults NOTIFY searchDone)

public:
    // Constructor
    explicit PhenotypesManager(QObject* parent=nullptr);

    // Getters
    inline HpoDataListModel* searchResults() const { return mSearchResults; }

    // Methods
    Q_INVOKABLE HpoData* getOrCreate(QString hpoId);
    Q_INVOKABLE void search(QString query);


Q_SIGNALS:
    void searchDone(bool success);

private:
    HpoDataListModel* mSearchResults = nullptr;
    QHash<QString, HpoData*> mHpoData;
    QHash<QString, QString> mPhenotypesSearchMap;
};

#endif // PHENOTYPESMANAGER_H