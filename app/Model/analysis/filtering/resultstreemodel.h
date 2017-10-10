#ifndef RESULTSTREEMODEL_H
#define RESULTSTREEMODEL_H

#include "Model/treemodel.h"
#include "filteringanalysis.h"
#include "annotation.h"

class FilteringAnalysis;

class ResultsTreeModel : public TreeModel
{
    Q_OBJECT
    Q_PROPERTY(bool isLoading READ isLoading WRITE setIsLoading NOTIFY isLoadingChanged)
    Q_PROPERTY(int total READ total WRITE setTotal NOTIFY totalChanged)
    Q_PROPERTY(int loaded READ loaded WRITE setLoaded NOTIFY loadedChanged)

public:
    explicit ResultsTreeModel(FilteringAnalysis* parent=nullptr);

    // QAbstractItemModel interface - lazy loading methods
    QHash<int, QByteArray> roleNames() const override;
    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex& parent) override;
//    bool hasChildren(const QModelIndex &parent) const Q_DECL_OVERRIDE;

    // Getters
    inline bool isLoading() { return mIsLoading; }
    inline int total() { return mTotal; }
    inline int loaded() { return mLoaded; }

    // Setters
    Q_INVOKABLE inline void setIsLoading(bool isLoading) { mIsLoading = isLoading; emit isLoadingChanged(); }
    inline void setTotal(int total) { mTotal = total; emit totalChanged(); }
    inline void setLoaded(int loaded) { mLoaded = loaded; emit loadedChanged(); }


    // Methods
    //! Force the Treemodel to reload data according to the provided filter
    Q_INVOKABLE void applyFilter(QJsonArray filter);
    //! To use when new columns have been added, to add info in the model without reseting it
    Q_INVOKABLE void reload();
    //! Load next result according to the mResultsPagination value (default is 100)
    Q_INVOKABLE void loadNext();


    QVariant newResultsTreeViewItem(Annotation* annot, QString uid, const QJsonValue &value);
    void setupModelData(QJsonArray data, TreeItem *parent);
    void loadAnalysisData();
    void initAnalysisData(int analysisId);
    bool fromJson(QJsonObject json, bool clearBefore=true);


Q_SIGNALS:
    void isLoadingChanged();
    void totalChanged();
    void loadedChanged();

private:
    FilteringAnalysis* mFilteringAnalysis;
    bool mIsLoading;
    int mAnalysisId;
    int mTotal;
    int mLoaded;
    int mPagination;
};

#endif // RESULTSTREEMODEL_H
