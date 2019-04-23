#ifndef __DEALDEVICE_H_
#define __DEALDEVICE_H_

#include <QObject>
#include <QDebug>
#include <QByteArray>
#include <QDomDocument>
#include "mediaUpLoad.h"
#include "accessToken.h"
#include "tokenGet.h"
#include "deviceAuth.h"

class DealDevice : public QObject
{
    Q_OBJECT
public:
    DealDevice();
    ~DealDevice();

    QString dealTextMessage(QDomDocument xml);
    QString createText(QString fromUserName, QString toUserName, QString msgType, QString text);

    QString dealImageMessage(QDomDocument xml);
    QString createImage(QString fromUserName, QString toUserName, QString msgType, QString mediaId);
    QString createVideo(QString fromUserName, QString toUserName, QString msgType, QString mediaId);

    QString dealVideoMessage(QDomDocument xml);
    QString dealEventMessage(QDomDocument xml);
    QString dealTextMessage1(QDomDocument xml);

    void messageJoint(QString httpBody);

signals:
    void messageReady(QString);

public slots:
    void messageRecv(QString recvData, int socket);

private:
    DeviceAuth dev;
};

#endif // DEALDEVICE_H
