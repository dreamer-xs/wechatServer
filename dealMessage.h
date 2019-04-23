#ifndef __DEALMESSAGE_H_
#define __DEALMESSAGE_H_

#include <QObject>
#include <QDebug>
#include <QByteArray>
#include <QDomDocument>
#include <QJsonDocument>
#include <QJsonObject>
#include "mediaUpLoad.h"
#include "httpClient.h"
#include "accessToken.h"
#include "tokenGet.h"
#include "userAuth.h"

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
    QString createVideo(QString fromUserName, QString toUserName, QString msgType, QString mediaId);

    QString dealVideoMessage(QDomDocument xml);
    QString dealEventMessage(QDomDocument xml);
    QString dealTextMessage1(QDomDocument xml);

    void messageJoint(QString httpBody);
	QString getJsonValue(QString data, QString key);

signals:
    void messageReady(QString);

public slots:
    void messageRecv(QString recvData);

private:

};

#endif // DEALMESSAGE_H
