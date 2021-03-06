#ifndef REGOVAR_H
#define REGOVAR_H

#include <QSettings>
//#include <QNetworkReply>
#include <QAuthenticator>
#include <QQmlApplicationEngine>

#include "mainmenu/rootmenu.h"
#include "framework/networkmanager.h"
#include "user/usersmanager.h"
#include "project/projectsmanager.h"
#include "subject/subjectsmanager.h"
#include "subject/samplesmanager.h"
#include "file/filesmanager.h"
#include "analysis/analysesmanager.h"
#include "tools/toolsmanager.h"
#include "subject/reference.h"
#include "panel/panelsmanager.h"
#include "event/eventsmanager.h"
#include "pipeline/pipelinesmanager.h"
#include "phenotype/phenotypesmanager.h"
#include "serverTask/servertaskslistmodel.h"

// TODO: rework as manager pattern
#include "user/user.h"
#include "admin.h"
#include "settings.h"


#ifndef regovar
#define regovar (Regovar::i())
#endif



class RegovarInfo;


/*!
 * \brief Singleton.
 * Main Regovar's client core. Wrap models and manage all interaction with the server
 * (websocket, rest api, tus resumable upload, and so on.
 */
class Regovar : public QObject
{
    Q_OBJECT

    // Welcom
    Q_PROPERTY(bool loaded READ loaded NOTIFY neverChanged)
    Q_PROPERTY(QString searchRequest READ searchRequest WRITE setSearchRequest NOTIFY searchRequestChanged)
    Q_PROPERTY(QJsonObject searchResult READ searchResult NOTIFY searchResultChanged)
    Q_PROPERTY(bool searchInProgress READ searchInProgress NOTIFY searchInProgressChanged)
    Q_PROPERTY(AnalysesListModel* lastAnalyses READ lastAnalyses NOTIFY lastDataChanged)
    Q_PROPERTY(SubjectsListModel* lastSubjects READ lastSubjects NOTIFY lastDataChanged)
    Q_PROPERTY(EventsListModel* lastEvents READ lastEvents NOTIFY neverChanged)
    Q_PROPERTY(bool welcomIsLoading READ welcomIsLoading WRITE setWelcomIsLoading NOTIFY welcomIsLoadingChanged)
    // Managers
    Q_PROPERTY(NetworkManager* networkManager READ networkManager NOTIFY neverChanged)
    Q_PROPERTY(UsersManager* usersManager READ usersManager NOTIFY neverChanged)
    Q_PROPERTY(ProjectsManager* projectsManager READ projectsManager NOTIFY neverChanged)
    Q_PROPERTY(SubjectsManager* subjectsManager READ subjectsManager NOTIFY neverChanged)
    Q_PROPERTY(SamplesManager* samplesManager READ samplesManager NOTIFY neverChanged)
    Q_PROPERTY(FilesManager* filesManager READ filesManager NOTIFY neverChanged)
    Q_PROPERTY(AnalysesManager* analysesManager READ analysesManager NOTIFY neverChanged)
    Q_PROPERTY(PanelsManager* panelsManager READ panelsManager NOTIFY neverChanged)
    Q_PROPERTY(PhenotypesManager* phenotypesManager READ phenotypesManager NOTIFY neverChanged)
    Q_PROPERTY(EventsManager* eventsManager READ eventsManager NOTIFY neverChanged)
    Q_PROPERTY(ToolsManager* toolsManager READ toolsManager NOTIFY neverChanged)
    Q_PROPERTY(PipelinesManager* pipelinesManager READ pipelinesManager NOTIFY neverChanged)

    // Others
    Q_PROPERTY(RootMenu* mainMenu READ mainMenu NOTIFY neverChanged)
    Q_PROPERTY(Settings* settings READ settings NOTIFY settingsChanged)
    Q_PROPERTY(QList<QObject*> references READ references NOTIFY referencesChanged)
    Q_PROPERTY(ServerTasksListModel* serverTasks READ serverTasks NOTIFY neverChanged)

    Q_PROPERTY(RegovarInfo* config READ config NOTIFY configChanged)
    Q_PROPERTY(Admin* admin READ admin NOTIFY adminChanged)

    // TODO: rework as Manager:
    //  - ConnectionManager (manage login, ServerStatus, pending queries, and websocket realtime event)
    //  - Configmanager ? (admin and settings)



public:
    static Regovar* i();
    void init();

    // Getters
    inline bool loaded() const { return mLoaded; }
    inline RegovarInfo* config() const { return mConfig; }
    inline Admin* admin() { return mAdmin; }
    //--
    inline QString searchRequest() { return mSearchRequest; }
    inline QJsonObject searchResult() const { return mSearchResult; }
    inline bool searchInProgress() const { return mSearchInProgress; }
    inline AnalysesListModel* lastAnalyses() const { return mLastAnalyses; }
    inline SubjectsListModel* lastSubjects() const { return mLastSubjects; }
    inline EventsListModel*  lastEvents() const { return mEventsManager->lastEvents(); }
    inline bool welcomIsLoading() const { return mWelcomIsLoading; }
    //--
    inline NetworkManager* networkManager() const { return mNetworkManager; }
    inline UsersManager* usersManager() const { return mUsersManager; }
    inline ProjectsManager* projectsManager() const { return mProjectsManager; }
    inline SubjectsManager* subjectsManager() const { return mSubjectsManager; }
    inline SamplesManager* samplesManager() const { return mSamplesManager; }
    inline FilesManager* filesManager() const { return mFilesManager; }
    inline AnalysesManager* analysesManager() const { return mAnalysesManager; }
    inline PanelsManager* panelsManager() const { return mPanelsManager; }
    inline PhenotypesManager* phenotypesManager() const { return mPhenotypesManager; }
    inline EventsManager* eventsManager() const { return mEventsManager; }
    inline ToolsManager* toolsManager() const { return mToolsManager; }
    inline PipelinesManager* pipelinesManager() const { return mPipelinesManager; }
    //--
    inline QList<QObject*> references() const { return mReferences; }
    inline RootMenu* mainMenu() const { return mMainMenu; }
    inline Settings* settings() const { return mSettings; }
    inline ServerTasksListModel* serverTasks() const { return mServerTasks; }

    // Setters
    inline void setSearchRequest(QString searchRequest) { mSearchRequest = searchRequest; emit searchRequestChanged(); }
    inline void setSearchResult(QJsonObject searchResult) { mSearchResult = searchResult; emit searchResultChanged(); }
    inline void setSearchInProgress(bool flag) { mSearchInProgress = flag; emit searchInProgressChanged(); }
    inline void setQmlEngine (QQmlApplicationEngine* engine) { mQmlEngine = engine; }
    inline void setWelcomIsLoading(bool flag) { mWelcomIsLoading=flag; emit welcomIsLoadingChanged(); }

    // Methods
    Q_INVOKABLE Reference* referenceFromId(int id);
    // Others
    Q_INVOKABLE inline void openNewProjectWizard() { emit newProjectWizardOpen(); }
    Q_INVOKABLE inline void openNewAnalysisWizard() { emit newAnalysisWizardOpen(); }
    Q_INVOKABLE inline void openNewSubjectWizard() { emit newSubjectWizardOpen(); }
    Q_INVOKABLE inline void openNewFileWizard() { emit newFileWizardOpen(); }
    Q_INVOKABLE inline void openServerTasksWindow() { openNewWindow(QUrl("qrc:/qml/Windows/ServerTasksWindow.qml"), nullptr); }
    Q_INVOKABLE void getFileInfo(int fileId);
    Q_INVOKABLE void getGeneInfo(QString geneName, int analysisId=-1);
    Q_INVOKABLE void getPanelInfo(QString panelId);
    Q_INVOKABLE void getPhenotypeInfo(QString phenotypeId);
    Q_INVOKABLE void getPipelineInfo(int pipelineId);
    Q_INVOKABLE void getSampleInfo(int sampleId);
    Q_INVOKABLE void getUserInfo(int userId);
    Q_INVOKABLE void getVariantInfo(int refId, QString variantId, int analysisId=-1);
    Q_INVOKABLE void search(QString query);
    Q_INVOKABLE void loadConfigData();
    Q_INVOKABLE void loadWelcomData();
    Q_INVOKABLE void close();
    void serverNotificationReceived(QString action, QJsonObject data);
    // Tools
    Q_INVOKABLE inline QUuid generateUuid() { return QUuid::createUuid(); }
    Q_INVOKABLE void manageServerError(QJsonObject json, QString method="");
    Q_INVOKABLE void manageClientError(QString msg, QString code="", QString method="");
    Q_INVOKABLE QDateTime dateFromString(QString date);
    Q_INVOKABLE QString formatNumber(int value);
    Q_INVOKABLE QString formatNumber(double value);
    Q_INVOKABLE QString formatDate(QDateTime date, bool withTime=true);
    Q_INVOKABLE QString formatDate(QString isodate, bool withTime=true);
    Q_INVOKABLE QString formatDuration(int duration);
    Q_INVOKABLE QString formatFileSize(qint64 size, qint64 uploadOffset=-1);
    Q_INVOKABLE inline QString analysisStatusLabel(QString status) { return Analysis::statusLabel(status); }
    Q_INVOKABLE inline QString analysisStatusIcon(QString status) { return Analysis::statusIcon(status); }
    Q_INVOKABLE inline bool analysisStatusIconAnimated(QString status) { return Analysis::statusIconAnimated(status); }
    bool openNewWindow(QUrl qmlUrl, QObject* model);
    bool openNewWindow(QUrl qmlUrl, QObject* model, QString wid);
    Q_INVOKABLE bool closeWindow(QString wid);
    Q_INVOKABLE inline QObject* getWindowModels(QString wid) const { if (mOpenWindowModels.contains(wid)) return mOpenWindowModels[wid]; return nullptr; }
    Q_INVOKABLE void restart();


Q_SIGNALS:
    //! special signal used for QML property that never changed to avoid to declare to many useless signal
    //! QML need that property declare a "changed" event for binding
    void neverChanged();
    // Omnisearch events
    void searchRequestChanged();
    void searchResultChanged();
    void searchInProgressChanged();
    // Misc events
    void welcomIsLoadingChanged();
    void lastDataChanged();
    void settingsChanged();
    void referencesChanged();
    void configChanged();
    void adminChanged();
    void focusOnWindow(QString wid);

    // Wizards events
    void newProjectWizardOpen();
    void newAnalysisWizardOpen();
    void newSubjectWizardOpen();
    void newFileWizardOpen();
    void serverTasksWindowOpen();
    // Infos panels events
//    void fileInformationSearching();
//    void geneInformationSearching();
//    void panelInformationSearching();
//    void phenotypeInformationSearching();
//    void diseaseInformationSearching();
//    void pipelineInformationSearching();
//    void sampleInformationSearching();
//    void userInformationSearching();
//    void variantInformationSearching();

    void fileInformationReady(File* file);
    void panelInformationReady(Panel* panel);
    void sampleInformationReady(Sample* sample);
    void userInformationReady(User* user);
    void pipelineInformationReady(Pipeline* pipeline);
    void geneInformationReady(Gene* gene);
    void phenotypeInformationReady(Phenotype* phenotype);
    void diseaseInformationReady(Disease* disease);
    void variantInformationReady(QJsonValue json);

    void errorOccured(QString errCode, QString message, QString techData);
    void onClose();

private:
    // Singleton pattern
    Regovar();
    ~Regovar();
    static Regovar* mInstance;

    // Models
    //! true when the model have been loaded (with loadWelcome)
    bool mLoaded = false;
    //! wrap all settings retrieved from QSetting before initialisation of all managers
    Settings* mSettings = nullptr;
    //! The config retrieved from the server
    RegovarInfo* mConfig = nullptr;
    //! Admin operation wrapper
    Admin* mAdmin = nullptr;
    //! Model of the main menu
    RootMenu* mMainMenu = nullptr;
    //! List of references supported by the server
    QList<QObject*> mReferences;
    //! List of tasks running on the server (realtime notification)
    ServerTasksListModel* mServerTasks = nullptr;
    //! Welcom last data
    AnalysesListModel* mLastAnalyses = nullptr;
    SubjectsListModel* mLastSubjects = nullptr;
    //! Search request and results
    QString mSearchRequest;
    QJsonObject mSearchResult;
    bool mSearchInProgress = false;
    bool mWelcomIsLoading = false;

    // Managers
    //! Manage network (all requests Up/Down with the server, authent, and websocket connection)
    NetworkManager* mNetworkManager = nullptr;
    //! Manage users
    UsersManager* mUsersManager = nullptr;
    //! Manage projects (Browse + CRUD)
    ProjectsManager* mProjectsManager = nullptr;
    //! Manage subjects (Browse + CRUD)
    SubjectsManager* mSubjectsManager = nullptr;
    //! Browse all samples available on the server
    SamplesManager* mSamplesManager = nullptr;
    //! Browse&Upload files
    FilesManager* mFilesManager = nullptr;
    //! Manage analyses
    AnalysesManager* mAnalysesManager = nullptr;
    //! Manage genes panels
    PanelsManager* mPanelsManager = nullptr;
    //! Manage phenotype
    PhenotypesManager* mPhenotypesManager = nullptr;
    //! Manage all events
    EventsManager* mEventsManager = nullptr;
    //! Custom Tools managers (exporters, reporters)
    ToolsManager* mToolsManager = nullptr;
    //! PipelinesManangers
    PipelinesManager* mPipelinesManager = nullptr;

    // Technical stuff
    //! We need ref to the QML engine to create/open new windows from model events
    QQmlApplicationEngine* mQmlEngine = nullptr;
    //! List of model used by open qml windows
    QHash<QString, QObject*> mOpenWindowModels;


    // Internals list of push notification key, to know which manager handle them
    QStringList mWsFilesActionsList = {"file_upload"};
    QStringList mWsSamplesActionsList = {"import_vcf_start", "import_vcf_processing", "import_vcf_end"};
    QStringList mWsFilteringActionsList = {"analysis_computing", "wt_update", "wt_creation", "filter_update", "filtering_prepare"};
    QStringList mWsFilterActionsList = {"filter_update"};
    QStringList mWsPipelinesActionsList = {"pipeline_install", "pipeline_uninstall"};
    QStringList mWsJobsActionsList = {"job_updated"};
    QStringList mWsEventsActionsList = {"new_event"};
};





/*!
 * \brief Class to old server's information
 * (to avoid to polluate main regovar singleton with these data)
 */
class RegovarInfo: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString serverVersion READ serverVersion NOTIFY configChanged)
    Q_PROPERTY(QString clientVersion READ clientVersion NOTIFY configChanged)
    Q_PROPERTY(QString website READ website NOTIFY configChanged)
    Q_PROPERTY(QString license READ license  NOTIFY configChanged)
    Q_PROPERTY(QJsonObject release READ release  NOTIFY configChanged)
    Q_PROPERTY(QString welcomMessage READ welcomMessage NOTIFY configChanged)
    Q_PROPERTY(QString welcomMessageType READ welcomMessageType NOTIFY configChanged)

public:
    // Constructor
    RegovarInfo(QObject *parent = nullptr);

    // Getters
    inline QString serverVersion() { return mServerVersion; }
    inline QString clientVersion() { return mClientVersion; }
    inline QString website() { return mWebsite; }
    inline QString license() { return mLicense; }
    inline QJsonObject release() { return mRelease; }
    inline QString welcomMessage() const { return mWelcomMessage; }
    inline QString welcomMessageType() const { return mWelcomMessageType; }

    // Setters
    inline void setRelease(QJsonObject release) { mRelease = release; emit configChanged(); }

    // Methods
    bool loadJson(QJsonObject json);

Q_SIGNALS:
    void configChanged();

private:
    QString mServerVersion;
    QString mClientVersion;
    QString mWebsite;
    QString mLicense;
    QJsonObject mRelease;
    //! Server welcom custom message
    QString mWelcomMessage = "";
    QString mWelcomMessageType = "";
};

#endif // REGOVAR_H
