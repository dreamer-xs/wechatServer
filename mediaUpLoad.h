#ifndef MEDIAUPLOAD_H
#define MEDIAUPLOAD_H

#include <QDebug>
#include <QThread>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QFile>



class MediaUpLoad : public QThread
{
    Q_OBJECT

public:
    MediaUpLoad(QString m_token, QString m_type, QString m_path);
    MediaUpLoad();
    ~MediaUpLoad();
    void run();
    QString mediaIdGet();

    QString uploadMedia(QString token, QString type, QString path);
    QString dealData(QString data);

signals:
    void mediaUpLoadFinished(QString type, QString mediaId);

private slots:
    void mediaGet(QString type, QString mediaId);

private:
    QString type;
    QString token;
    QString mediaPath;
};

#endif // MEDIAUPLOAD_H
