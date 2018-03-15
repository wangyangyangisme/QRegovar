#ifndef PIPELINEANALYSIS_H
#define PIPELINEANALYSIS_H

#include <QtCore>
#include "Model/analysis/analysis.h"
#include "Model/pipeline/pipeline.h"
#include "Model/file/fileslistmodel.h"

class PipelineAnalysis: public Analysis
{
    Q_OBJECT
    // Regovar resource attribute
    Q_PROPERTY(bool loaded READ loaded NOTIFY dataChanged)
    Q_PROPERTY(QDateTime updateDate READ updateDate NOTIFY dataChanged)
    Q_PROPERTY(QDateTime createDate READ createDate NOTIFY dataChanged)
    // PipelineAnalysis (Job) attribute
    Q_PROPERTY(int id READ id NOTIFY neverChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY dataChanged)
    Q_PROPERTY(QString comment READ comment WRITE setComment NOTIFY dataChanged)
    Q_PROPERTY(QJsonObject config READ config NOTIFY neverChanged)
    Q_PROPERTY(JobStatus status READ status NOTIFY dataChanged)
    Q_PROPERTY(double progressValue READ progressValue NOTIFY dataChanged)
    Q_PROPERTY(QString progressLabel READ progressLabel NOTIFY dataChanged)

    Q_PROPERTY(Pipeline* pipeline READ pipeline WRITE setPipeline NOTIFY pipelineChanged)
    Q_PROPERTY(FilesListModel* inputsFiles READ inputsFiles NOTIFY neverChanged)
    Q_PROPERTY(FilesListModel* outputsFiles READ outputsFiles NOTIFY neverChanged)

public:
    enum JobStatus
    {
        Waiting=0,
        Initializing,
        Running,
        Pause,
        Finalizing,
        Done,
        Canceled,
        Error
    };
    Q_ENUM(JobStatus)

    // Constructors
    explicit PipelineAnalysis(QObject* parent=nullptr);
    explicit PipelineAnalysis(int id, QObject* parent=nullptr);

    // Getters
    inline bool loaded() const { return mLoaded; }
    inline QDateTime updateDate() const { return mUpdateDate; }
    inline QDateTime createDate() const { return mCreateDate; }
    inline int id() const { return mId; }
    inline QString name() const { return mName; }
    inline QString comment() const { return mComment; }
    inline QJsonObject config() const { return mConfig; }
    inline JobStatus status() const { return mStatus; }
    inline double progressValue() const { return mProgressValue; }
    inline QString progressLabel() const { return mProgressLabel; }
    inline Pipeline* pipeline() const { return mPipeline; }
    inline FilesListModel* inputsFiles() const { return mInputsFiles; }
    inline FilesListModel* outputsFiles() const { return mOutputsFiles; }

    // Setters
    inline void setName(QString name) { mName = name; emit dataChanged(); }
    inline void setComment(QString comment) { mComment = comment; emit dataChanged(); }
    void setPipeline(Pipeline* pipe);
    void setStatus(JobStatus status);
    void setStatus(QString status);


    // Analysis Abstracty Methods overriden
    //! Set model with provided json data
    Q_INVOKABLE bool fromJson(QJsonObject json, bool full_init=false);
    //! Export model data into json object
    Q_INVOKABLE QJsonObject toJson();
    //! Save subject information onto server
    Q_INVOKABLE void save();
    //! Load Subject information from server
    Q_INVOKABLE void load(bool forceRefresh=true);

    // Methods
    Q_INVOKABLE void addInputs(QList<QObject*> inputs);
    Q_INVOKABLE void removeInputs(QList<QObject*> inputs);
    Q_INVOKABLE void addInputFromWS(QJsonObject json);
    //! Control the execution of the job (if supported by the container technology)
    Q_INVOKABLE void pause();
    Q_INVOKABLE void start();
    Q_INVOKABLE void cancel();
    Q_INVOKABLE void finalyze();
    Q_INVOKABLE void refreshMonitoring();

public Q_SLOTS:
    void processPushNotification(QString action, QJsonObject data);

Q_SIGNALS:
    void neverChanged();
    void pipelineChanged();
    void dataChanged();


private:
    bool mLoaded = false;
    QDateTime mUpdateDate;
    QDateTime mCreateDate;
    QDateTime mLastInternalLoad = QDateTime::currentDateTime();

    // Attributes
    int mId = -1;
    QString mName;
    QString mComment;
    QJsonObject mConfig;
    JobStatus mStatus = Waiting;
    double mProgressValue = 0;
    QString mProgressLabel;

    //! The pipeline used for the analysis
    Pipeline* mPipeline = nullptr;
    //! The list of files used as input for the analysis
    FilesListModel* mInputsFiles = nullptr;
    //! The list of files created by the analysis
    FilesListModel* mOutputsFiles = nullptr;

};

#endif // PIPELINEANALYSIS_H
