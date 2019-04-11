#include "httpServer.h"

HttpServer::HttpServer()
{
    m_piserver = new QTcpServer;
    m_pisocket = new QTcpSocket;
    StartListen(80);
}
HttpServer::~HttpServer()
{
    if(m_piserver)
        delete(m_piserver);
    if(m_pisocket)
        delete(m_pisocket);
}

void HttpServer::StartListen(int nPort)
{
    if(m_piserver->listen(QHostAddress::Any,nPort))
        qDebug() << "listen ok";
    else
        qDebug() << "listen err";
    connect(m_piserver,SIGNAL(newConnection()),this,SLOT(newClientConnect()));
}

void HttpServer::newClientConnect()
{
    qDebug() << "new client connect";
    m_pisocket = m_piserver->nextPendingConnection();
    connect(m_pisocket,SIGNAL(readyRead()),this,SLOT(readMessage()));
    connect(m_pisocket,SIGNAL(disconnected()),this,SLOT(disConnect()));
}

void HttpServer::sendMessage(QString data)
{
    qDebug()<<"----------------发送数据--------------------";
    qDebug()<<data;
    qDebug()<<"--------------------------------------------";
    QByteArray httpData;
    httpData.append(data);

    m_pisocket->write(httpData);
}


void HttpServer::readMessage()
{
    //延时100ms，防止未接收完全
    usleep(1000*100);

    QByteArray buf;
    buf = m_pisocket->readAll();

    QString recvData(buf);
    qDebug()<<"---------------原始接收数据-----------------";
    qDebug()<<recvData;
    qDebug()<<"--------------------------------------------";
    emit messageRecived(recvData);
}

void HttpServer::disConnect()
{
    qDebug() << "client disconnect";
}

