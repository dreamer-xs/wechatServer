#include "tcpsocket.h"
#include <QtConcurrent/QtConcurrent>
#include <QHostAddress>
#include <QDebug>

TcpSocket::TcpSocket(qintptr socketDescriptor, QObject *parent) : //构造函数在主线程执行，lambda在子线程
    QTcpSocket(parent),socketID(socketDescriptor)
{
    this->setSocketDescriptor(socketDescriptor);
    connect(this,&TcpSocket::readyRead,this,&TcpSocket::readData);
    dis = connect(this,&TcpSocket::disconnected,
        [&](){
            //qDebug() << "disconnect"<<" port: "<<this->localPort()<< "socketDescriptor: "<< this->socketDescriptor();
            qDebug() << "disconnect"<<" port: "<<this->peerPort()<< "socketDescriptor: "<< this->socketDescriptor();
            emit sockDisConnect(socketID,this->peerAddress().toString(),this->peerPort(),QThread::currentThread());//发送断开连接的用户信息
            this->deleteLater();
        });
    connect(&watcher,&QFutureWatcher<QByteArray>::finished,this,&TcpSocket::startNext);
    connect(&watcher,&QFutureWatcher<QByteArray>::canceled,this,&TcpSocket::startNext);
    //qDebug() << "new connect" <<" port: "<<this->localPort()<< "socketDescriptor: "<< this->socketDescriptor();
    qDebug() << "new connect" <<" port: "<<this->peerPort()<< "socketDescriptor: "<< this->socketDescriptor();
}

TcpSocket::~TcpSocket()
{
}


void TcpSocket::sentData(const QByteArray &data, const int id)
{
    if(id == socketID)
    {
        write(data);
    }
}

void TcpSocket::disConTcp(int i)
{
    if (i == socketID)
    {
        this->disconnectFromHost();
    }
    else if (i == -1) //-1为全部断开
    {
        disconnect(dis); //先断开连接的信号槽，防止二次析构
        this->disconnectFromHost();
        this->deleteLater();
    }
}

void TcpSocket::writeData(QString data)
{
    this->write(data.toUtf8());
}
void TcpSocket::readData()
{
    //延时100ms，防止未接收完全
    //usleep(1000*100);
    QElapsedTimer tm;
    tm.start();
    while(tm.elapsed() < 100)
    {}

    //auto data  = handleData(this->readAll(),this->peerAddress().toString(),this->peerPort());
    auto data  = handleData(this->readAll(),this->peerAddress().toString(),this->localPort());
    //this->write(data);
//    if (!watcher.isRunning())//放到异步线程中处理。
//    {
//        watcher.setFuture(QtConcurrent::run(this,&TcpSocket::handleData,datas.dequeue(),this->peerAddress().toString(),this->peerPort()));
//    }
}


QByteArray TcpSocket::handleData(QByteArray data, const QString &ip, qint16 port)
{
    QElapsedTimer tm;
    tm.start();
    while(tm.elapsed() < 100)
    {}

    qDebug()<<"---------------原始接收数据-----------------";
    QString recvData;
    recvData.append(data);
    int socket = this->socketDescriptor();
    qDebug()<<"port: "<<port << "socketDescriptor: "<< socket;
    qDebug()<<recvData;
    qDebug()<<"--------------------------------------------";

    //微信服务器请求
    //微信请求后将用户ID与设备ID关联并存入数据库
    {
        qDebug()<<"wechat";
        // 对数据的处理
        DealMessage d;
        connect(&d, SIGNAL(messageReady(QString)), this, SLOT(writeData(QString)));
        d.messageRecv(recvData);
    }
    return data;
}

void TcpSocket::startNext()
{
    this->write(watcher.future().result());
    if (!datas.isEmpty())
    {
        watcher.setFuture(QtConcurrent::run(this,&TcpSocket::handleData,datas.dequeue(),this->peerAddress().toString(),this->peerPort()));
    }
}
