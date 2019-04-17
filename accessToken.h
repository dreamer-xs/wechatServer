#ifndef ACCESSTOKEN_H
#define ACCESSTOKEN_H

#include <QDebug>
#include <QThread>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QFile>
#include "httpClient.h"
#include "dbOperation.h"
#include <unistd.h>



class AccessToken : public QThread
{
    Q_OBJECT

public:
    AccessToken();
    ~AccessToken();
    void run();

    QString tokenValue();
    void tokenGetFromCache();
    void tokenGetFromDB();
    int tokenSaveToCache(QString token);
    int tokenSaveToDB(QString token);
    QString tokenGetFromServer();

private slots:
    //void oneProcessFinished(QNetworkReply*);
	QString dealHttpData(QString data);

private:
    HttpClient *tokenClient;
    bool tokenValid;
    QString accessTokenValue;
    QString tokenFilePath;
    int tokenGetTime;
    DbOperation* db;
};

#endif // ACCESSTOKEN_H
