#ifndef __DEALMESSAGE_H_
#define __DEALMESSAGE_H_

#include <QObject>
#include <QDebug>
#include <QByteArray>
#include <QDomDocument>
#include "httpServer.h"
#include "mediaUpLoad.h"
#include "accessToken.h"
#include "tokenGet.h"

class DealMessage : public QObject
{
    Q_OBJECT
public:
    DealMessage();
    ~DealMessage();

    QString dealTextMessage(QDomDocument xml);
    QString createText(QString fromUserName, QString toUserName, QString msgType, QString text);

    QString dealImageMessage(QDomDocument xml);
    QString createImage(QString fromUserName, QString toUserName, QString msgType, QString mediaId);

    QString dealTextMessage1(QDomDocument xml);
    QString dealEventMessage(QDomDocument xml);

    void messageJoint(QString httpBody);

signals:
    void messageReady(QString);

public slots:
    void messageRecv(QString recvData);

private:
    HttpServer server;

};

#endif // DEALMESSAGE_H
