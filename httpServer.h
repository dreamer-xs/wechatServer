#ifndef __SERVER_H_
#define __SERVER_H_

#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QByteArray>
#include <QDomDocument>

class HttpServer : public QObject
{
    Q_OBJECT
public:
    explicit HttpServer(QObject *parent = nullptr);
    void sendMessage(QString);
    void dealMessage(QString);
    void dealTextMessage(QDomDocument xml);
    void dealTextMessage1(QDomDocument xml);
    void dealEventMessage(QDomDocument xml);
    void dealImageMessage(QDomDocument xml);
    void dealVoiceMessage(QDomDocument xml);

    void StartListen(int nPort);
private:
    QTcpServer *m_piserver;
    QTcpSocket *m_pisocket;

signals:

public slots:
    void newClientConnect();
    void readMessage();
    void disConnect();
};

#endif // SERVER_H
