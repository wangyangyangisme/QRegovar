#ifndef FILESMANAGER_H
#define FILESMANAGER_H

#include <QObject>
#include "file.h"
#include "filestreemodel.h"
#include "tusuploader.h"
#include "fileslistmodel.h"

class FilesManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(FilesListModel* uploadsList READ uploadsList NOTIFY uploadsChanged)
    Q_PROPERTY(int uploadsProgress READ uploadsProgress NOTIFY uploadsChanged)

    Q_PROPERTY(FilesListModel* remoteList READ remoteList NOTIFY remoteListChanged)
    //Q_PROPERTY(FilesTreeModel* filesTree READ filesTree NOTIFY filesTreeChanged)



public:
    // Constructor
    FilesManager(QObject *parent = nullptr);

    // Getters
    inline int uploadsProgress() const { return mUploadsProgress; }
    inline FilesListModel* remoteList() const { return mRemoteFilesList; }
    inline FilesListModel* uploadsList() const { return mUploadsList; }
    //inline FilesTreeModel* filesTree() const { return mFilesTree; }

    // Method
    Q_INVOKABLE File* getOrCreateFile(int id);
    Q_INVOKABLE File* getFile(int id);
    Q_INVOKABLE bool deleteFile(int id);
    Q_INVOKABLE void loadFilesBrowser();
    Q_INVOKABLE void enqueueUploadFile(QStringList filesPaths);
    Q_INVOKABLE void cancelUploadFile(QList<int> filesId);
    Q_INVOKABLE void pauseUpload(int id);
    Q_INVOKABLE void cancelUpload(int id);
    Q_INVOKABLE void startUpload(int id);
    void processPushNotification(QString action, QJsonObject json);
    void updateUploadProgress();
    void resumeUploads();


public Q_SLOTS:
    void filesEnqueued(QHash<QString,QString> mapping);


Q_SIGNALS:
    void uploadsChanged();
    void remoteListChanged();
    //void filesTreeChanged();
    void fileUploadEnqueued(QString localPath, int fileId);

private:
    //! internal hash map of all files. Will keep ref to all file loaded even if they are no more displayed
    QHash<int, File*> mFiles;
    //! The progress (0 to 100) of all uploads
    int mUploadsProgress = 0;
    //! The list of files that are currently uploadling (by this client, add file with enqueueUploadFile method)
    FilesListModel* mUploadsList = nullptr;
    //! The flat list of files available on the server (load/init with loadFilesBrowser method)
    FilesListModel* mRemoteFilesList = nullptr;
    //! The tree list of files available on the server (load/init with loadFilesBrowser method)
    //FilesTreeModel* mFilesTree = nullptr;
    //! The uploader that manage TUS protocol (resumable upload)
    TusUploader * mUploader = nullptr;
};

#endif // FILESMANAGER_H
