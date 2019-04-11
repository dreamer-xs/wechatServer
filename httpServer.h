#ifndef __SERVER_H_
#define __SERVER_H_

#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QByteArray>
#include <unistd.h>

class HttpServer : public QObject
{
    Q_OBJECT
public:
    HttpServer();
    ~HttpServer();

    void StartListen(int nPort);

signals:
    void messageRecived(QString recvData);

public slots:
    void sendMessage(QString data);
    void newClientConnect();
    void readMessage();
    void disConnect();

private:
    QTcpServer *m_piserver;
    QTcpSocket *m_pisocket;
};

#endif // SERVER_H
