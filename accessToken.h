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

/*
 *Accesstoken-> 独立获取并维护有效token线程
 *60s判断一次token是否过期
 *过期则从微信服务器获取新的token，并存入数据库中
 *
 *TokenGet   -> 获取有效token接口
 */


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
