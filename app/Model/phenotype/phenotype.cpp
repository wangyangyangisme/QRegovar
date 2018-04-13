#include "phenotype.h"
#include "Model/regovar.h"


Phenotype::Phenotype(QObject* parent) : HpoData(parent)
{
    mType = "phenotypic";
    mParents = new HpoDataListModel(this);
    mChilds = new HpoDataListModel(this);
    mDiseases = new DiseasesListModel(this);
    connect(this, &Phenotype::dataChanged, this, &Phenotype::updateSearchField);
}
Phenotype::Phenotype(QString hpo_id, QObject* parent) : Phenotype( parent)
{
    mId = hpo_id;
}


void Phenotype::updateSearchField()
{
    mSearchField = mId + " " + mLabel + " " + mDefinition;
    // TODO: add genes, diseases label
}

bool Phenotype::fromJson(QJsonObject json)
{
    mId = json["id"].toString();
    mLabel = json["label"].toString();

    if (!json.contains("definition"))
    {
        emit dataChanged();
        return true;
    }

    // Load full data
    mDefinition = json["definition"].toString();
    mCategory = json["category"].toString();
    if (json.contains("parents"))
    {
        mParents->clear();
        mChilds->clear();
        for(const QJsonValue& val: json["parents"].toArray())
        {
            QJsonObject parent = val.toObject();
            Phenotype* ppheno = (Phenotype*) regovar->phenotypesManager()->getOrCreate(parent["id"].toString());
            ppheno->fromJson(parent);
            mChilds->add(ppheno);
        }
        for(const QJsonValue& val: json["childs"].toArray())
        {
            QJsonObject child = val.toObject();
            Phenotype* cpheno = (Phenotype*) regovar->phenotypesManager()->getOrCreate(child["id"].toString());
            cpheno->fromJson(child);
            mChilds->add(cpheno);
        }
        mLoaded = true;
    }

    if (json.contains("genes"))
    {
        mGenes->fromJson(json["genes"].toArray());
    }
    if (json.contains("diseases"))
    {
        mDiseases->setPhenotypeId(mId);
        mDiseases->fromJson(json["diseases"].toArray());
    }

    if (json.contains("meta"))
    {
        mMeta = json["meta"].toObject();
        mGenesFreq = mMeta["genes_freq"].toObject();
        mDiseasesFreq = mMeta["diseases_freq"].toObject();
        mQualifiers = mMeta["qualifiers"].toObject();
        if (mMeta.contains("sources"))
        {
            mSources.clear();
            for(const QJsonValue& val: mMeta["sources"].toArray())
            {
                mSources.append(val.toString());
            }
        }
    }

    emit dataChanged();
    return true;
}



QString Phenotype::qualifier(QString diseaseId)
{
    QString result;
    if (mQualifiers.contains(diseaseId))
    {
        for (const QJsonValue& val: mQualifiers[diseaseId].toArray() )
        {
            QJsonObject data = val.toObject();
            if (data.contains("definition"))
                result += data["definition"].toString() + " ";
            else
                result += val.toString() + ". ";
        }
    }
    return result;
}
