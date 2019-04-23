#ifndef USERAUTH_H
#define USERAUTH_H

#include <QDebug>
#include <QThread>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QFile>
#include "httpClient.h"
#include "dbOperation.h"
#include <unistd.h>



class UserAuth : public QThread
{
    Q_OBJECT

public:
    UserAuth();
    ~UserAuth();
    void run();
    bool isValid(QString userId);
    bool saveToDB(QString userId, QString deviceId);
    bool delFromDB(QString userId);
    int getSocket(QString userId);


private slots:

private:
    DbOperation* db;
};

#endif // USERAUTH_H
