#ifndef DEVICEAUTH_H
#define DEVICEAUTH_H

#include <QDebug>
#include <QThread>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QFile>
#include "httpClient.h"
#include "dbOperation.h"
#include <unistd.h>



class DeviceAuth : public QThread
{
    Q_OBJECT

public:
    DeviceAuth();
    ~DeviceAuth();
    void run();
    bool isValid(QString deviceId);
    bool saveToDB(QString id, int status, int socket);
    int getSocket(QString deviceId);


private slots:

private:
    DbOperation* db;
};

#endif // DEVICEAUTH_H
