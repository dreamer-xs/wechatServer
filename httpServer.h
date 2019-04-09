#ifndef __SERVER_H_
#define __SERVER_H_

#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QByteArray>
#include <QDomDocument>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    void sendMessage(QString);
    void dealMessage(QString);

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
