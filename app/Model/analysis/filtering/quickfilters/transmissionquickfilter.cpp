#include "transmissionquickfilter.h"
#include <QJsonArray>

// This quick filter is using several of precomputed regovar's annotations
// These annotations are done and use same fuid for all analaysis/ref
// fuid mapping
// is_dom           148e67ffc504d1d0baf8ec988a2b7c4e
// is_rec_hom       b0fab8b285474229afbbc13fac198dfe
// is_rec_htzcomp   8cb83d0127aa912f2d290139d298e082
// is_denovo        92a9da3488b1127623c4e3ac7b6f67e2
// is_inherited     7ea7055963560e3ba97bd5a0081fa66c
// is_aut           9b3a9330bc8b5b3c1ebc271876535d8e
// is_xlk           76117d8b774f5e902ba0580bc302afb0
// is_mit           0bfb15c0aad2926202f1ca0ee4e44c59

TransmissionQuickFilter::TransmissionQuickFilter(int) : QuickFilterBlockInterface()
{
    // Todo : build thid list according to avalable annotation db (in checkAnnotationsDB method)
    QStringList opList;
    opList.append("==");
    // Transmission
    mFilters["dom"] = new QuickFilterField("148e67ffc504d1d0baf8ec988a2b7c4e", tr("Dominant"), opList, "==", QVariant(true), false);
    mFilters["rec_hom"] = new QuickFilterField("b0fab8b285474229afbbc13fac198dfe", tr("Homozigous"), opList, "==", QVariant(true), false);
    mFilters["rec_htzcomp"] = new QuickFilterField("8cb83d0127aa912f2d290139d298e082", tr("Htz comp"), opList, "==", QVariant(true), false);
    // Inheritance
    mFilters["denovo"] = new QuickFilterField("92a9da3488b1127623c4e3ac7b6f67e2", tr("De novo"), opList, "==", QVariant(true), false);
    mFilters["inherited"] = new QuickFilterField("92a9da3488b1127623c4e3ac7b6f67e2", tr("Inherited"), opList, "==", QVariant(false), false);
    // Location
    mFilters["aut"] = new QuickFilterField("9b3a9330bc8b5b3c1ebc271876535d8e", tr("Autosomal"), opList, "==", QVariant(true), false);
    mFilters["xlk"] = new QuickFilterField("76117d8b774f5e902ba0580bc302afb0", tr("X-linked"), opList, "==", QVariant(true), false);
    mFilters["mit"] = new QuickFilterField("0bfb15c0aad2926202f1ca0ee4e44c59", tr("Mito"), opList, "==", QVariant(true), false);
}



bool TransmissionQuickFilter::isVisible()
{
    // This filter is always availble in the UI
    return true;
}


QJsonArray TransmissionQuickFilter::toJson()
{
    QJsonArray filter;
//    QJsonArray recFilter;
//    foreach(QString fid, mActiveFilters.keys())
//    {
//        if (mActiveFilters[fid])
//        {
//            if (fid == "rec_hom" || fid == "rec_htzcomp")
//            {
//                recFilter.append();
//                recFilter << mFilters[fid].toJson();
//            }
//            else
//            {
//                filter << mFilters[fid];
//            }
//        }
//    }



//    QString resSubFilter = "";
//    if (recFilter.count() == 1)
//        resSubFilter = QString(", %1").arg(recFilter[0]);
//    else if (recFilter.count() > 1)
//        resSubFilter = QString(", [\"OR\", [%1]]").arg(recFilter.join(","));


//    QString resFilter = "";
//    if (filter.count() > 1 || (filter.count() == 1 && recFilter.count() > 0))
//        return QString("%1%2").arg(filter.join(","), resSubFilter);
//    else if (filter.count() == 1)
//        return filter[0];
//    else if (filter.count() == 0 && recFilter.count() > 0)
//        return resSubFilter.mid(2);
    return filter;
}



void TransmissionQuickFilter::setFilter(QString filterId, bool filterActive, QVariant)
{
    mFilters[filterId]->setIsActive(filterActive);
}



void TransmissionQuickFilter::clear()
{
    foreach(QString fid, mFilters.keys())
    {
        mFilters[fid]->setIsActive(false);
    }
}

void TransmissionQuickFilter::checkAnnotationsDB(QList<QObject*>)
{
}


bool TransmissionQuickFilter::loadJson(QJsonArray filter)
{
    // TODO or not TODO ?
    return false;
}
