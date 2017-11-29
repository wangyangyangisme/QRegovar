#include "file.h"

#include <QTextStream>
#include "Model/regovar.h"
#include "Model/framework/request.h"


QStringList File::zip = {"zip", "gz", "xz", "tar", "rar"};
QStringList File::txt = {"txt", "vcf", "sam", "fasta", "fastq", "csv"};
QStringList File::src = {"sh", "bat", "xml", "css", "py", "js", "html", "htm"};
QStringList File::aud = {"wav", "ogg", "mp3"};
QStringList File::vid = {"avi", "wmv", "mov", "mpg", "mpeg", "mkv"};
QStringList File::img = {"tiff", "tif", "gif", "jpeg", "jpg", "jpe", "png", "bmp"};
QStringList File::xls = {"xls", "xlsx", "ods"};
QStringList File::doc = {"doc", "docx", "odt"};
QStringList File::prz = {"ppt", "pps", "ppsx", "pptx", "odp"};
QStringList File::pdf = {"pdf", "ps"};



File::File(QObject* parent) : QObject(parent)
{
}
File::File(QJsonObject json, QObject* parent) : QObject(parent)
{
    fromJson(json);
}
File::File(int id, QObject* parent) : QObject(parent)
{
    mId = id;
}





void File::save()
{
    if (mId == -1) return;
    Request* request = Request::put(QString("/file/%1").arg(mId), QJsonDocument(toJson()).toJson());
    connect(request, &Request::responseReceived, [this, request](bool success, const QJsonObject& json)
    {
        if (success)
        {
            qDebug() << "File saved";
        }
        else
        {
            QJsonObject jsonError = json;
            jsonError.insert("method", Q_FUNC_INFO);
            regovar->raiseError(jsonError);
        }
        request->deleteLater();
    });
}



void File::load()
{
    Request* req = Request::get(QString("/file/%1").arg(mId));
    connect(req, &Request::responseReceived, [this, req](bool success, const QJsonObject& json)
    {
        if (success)
        {
            fromJson(json["data"].toObject());
        }
        else
        {
            QJsonObject jsonError = json;
            jsonError.insert("method", Q_FUNC_INFO);
            regovar->raiseError(jsonError);
        }
        req->deleteLater();
    });
}


bool File::fromJson(QJsonDocument json)
{
    QJsonObject data = json.object();
    return fromJson(data);
}


bool File::fromJson(QJsonObject json)
{

    mId = json["id"].toInt();
    setName(json["name"].toString());
    setComment(json["comment"].toString());
    mUrl = QUrl(json["path"].toString());
    mCreationDate = QDateTime::fromString(json["creation_date"].toString());
    mUpdateDate = QDateTime::fromString(json["update_date"].toString());
    emit updateDateChanged();
    setMd5Sum(json["md5sum"].toString());
    setType(json["type"].toString());
    setSize(json["size"].toInt());
    setUploadOffset(json["upload_offset"].toInt());

    // Build url to the local physical path
    QString folder = QDir::cleanPath(regovar->filesManager()->cacheDir()
            + QDir::separator() + "files"
            + QDir::separator() + QString::number(mId));

    mLocalPath = QDir::cleanPath(folder + QDir::separator() + mName);
    QDir dir(folder);
    if (!dir.exists())
    {
        dir.mkpath(".");
    }


    auto meta = QMetaEnum::fromType<FileStatus>();
    setStatus(static_cast<FileStatus>(meta.keyToValue(json["status"].toString().toStdString().c_str()))); // T_T .... tout ça pour ça ....

    QJsonObject filenameInfo;
    filenameInfo.insert("icon", extensionToIco(mType));
    filenameInfo.insert("filename", mName);
    mFilenameUI = QVariant::fromValue(filenameInfo);
    emit filenameUIChanged();

    QJsonObject statusInfo;
    statusInfo.insert("status", mStatus);
    statusInfo.insert("label", statusToLabel(mStatus, mSize, mUploadOffset));
    mStatusUI = QVariant::fromValue(statusInfo);
    emit statusUIChanged();

    mSizeUI = regovar->sizeToHumanReadable(mSize, mUploadOffset);
    emit sizeUIChanged();
    mSourceUI = (json["job_source_id"].toBool()) ? tr("Generated by analysis") : tr("Uploaded by user");
    emit sourceUIChanged();


    // TODO
    //mTags;
    //mSource;
    //mLocalPath;

    return true;
}



QJsonObject File::toJson()
{
    QJsonObject result;
    // Simples data
    result.insert("id", mId);
    result.insert("name", mName);
    result.insert("comment", mComment);
    // other data must be updated only by the server.
    return result;
}







// TODO: to redo with TUS protocol
bool File::downloadLocalFile()
{
    if (mStatus == uploaded || mStatus == checked)
    {
        if (QFile::exists(mLocalPath))
        {
            QFileInfo info(mLocalPath);
            mDownloadOffset = info.size();
            //if (mDownloadOffset == mSize)
            {
                mLocalFileReady = true;
                emit downloadOffsetChanged();
                emit localFileReadyChanged();
                emit localStatusChanged();
            }
//            else
//            {
//                // TODO: TUS: continue download
//                qDebug() << "ERROR: CACHED FILE, partial download, need to resume";
//            }
        }
        else
        {
            // Need to download file
            Request* req = Request::download(QString("/dl/file/%1/%2").arg(mId).arg(mName));
            connect(req, &Request::downloadReceived, [this, req](bool success, const QByteArray& data)
            {
                if (success)
                {
                    QFile* file = new QFile(mLocalPath);
                    file->open(QIODevice::WriteOnly);
                    file->write(data);
                    file->close();
                    mDownloadOffset = file->size();
                    mLocalFileReady = true;
                    emit downloadOffsetChanged();
                    emit localFileReadyChanged();
                    emit localStatusChanged();
                }
                else
                {
                    // TODO: create json error from scratch
                    QJsonObject jsonError;
                    jsonError.insert("method", Q_FUNC_INFO);
                    regovar->raiseError(jsonError);
                }
                req->deleteLater();
            });
        }
        return true;
    }


    qDebug() << "ERROR: file" << mId << "(" << mUrl << ") not ready to be retrieved. Wrong status : " << mStatus;
    return false;
}





QString File::readFile()
{
    if (!mLocalFileReady || !QFile::exists(mLocalPath))
    {
        return tr("File %1 doesn't exists. Please, download it first").arg(mLocalPath);
    }

    QFile* file = new QFile(mLocalPath);
    QString fileContent;
    if (file->open(QIODevice::ReadOnly))
    {
        QString line;
        QTextStream t(file);
        do
        {
            line = t.readLine();
            fileContent += line + "\n";
        } while (!line.isNull());

        file->close();
        file->deleteLater();
    }
    else
    {
        qDebug() << "Unable to open the file :" << mLocalPath;
        return QString();
    }
    return fileContent;
}









QString File::extensionToIco(QString ext)
{
    if (zip.contains(ext)) return "P";
    if (txt.contains(ext)) return "Y";
    if (img.contains(ext)) return "T";
    if (src.contains(ext)) return "R";
    if (aud.contains(ext)) return "Q";
    if (vid.contains(ext)) return "Z";
    if (xls.contains(ext)) return "S";
    if (doc.contains(ext)) return "0";
    if (prz.contains(ext)) return "W";
    if (pdf.contains(ext)) return "V";
    return "U";
}





QString File::statusToLabel(FileStatus status, qint64 size, qint64 uploadOffset)
{

    if(status == uploading)
    {
        QString progress;
        double s = size;
        if (s>0) progress = QString::number( uploadOffset/s*100, 'f', 2 );
        return tr("Uploading") + QString(" (%1%)").arg(progress);
    }
    if(status == checked) return tr("Checked");
    if(status == uploaded) return tr("Uploaded");
    return tr("Error");
}
