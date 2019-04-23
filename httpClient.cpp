#include "httpClient.h"

HttpClient::HttpClient()
{
    httpRecvData.clear();
}

HttpClient::~HttpClient()
{
}

QString HttpClient::get(QString urlTmp)
{
    httpRecvData.clear();
    url = urlTmp;

    QEventLoop temp_loop;
    manager = new QNetworkAccessManager(this);
    QNetworkReply* reply = manager->get(QNetworkRequest(QUrl(url)));
    connect(reply, SIGNAL(finished()), &temp_loop, SLOT(quit()));   //事件循环等待到收到信号
    temp_loop.exec();

    QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    QVariant redirectionTargetUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if(reply->error()==QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();
        httpRecvData = QString::fromUtf8(bytes);
        //qDebug()<<httpRecvData;
    }
    else
    {
        qDebug()<<"reply error!";
    }
    reply->deleteLater();

    return httpRecvData;
}


QString HttpClient::post(QString urlTmp, QString data)
{
    httpRecvData.clear();
    url = urlTmp;

    QEventLoop temp_loop;
    manager = new QNetworkAccessManager(this);
    QNetworkReply* reply = manager->post(QNetworkRequest(QUrl(url)),data.toUtf8());
    connect(reply, SIGNAL(finished()), &temp_loop, SLOT(quit()));   //事件循环等待到收到信号
    temp_loop.exec();

    QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    QVariant redirectionTargetUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if(reply->error()==QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();
        httpRecvData = QString::fromUtf8(bytes);
        //qDebug()<<httpRecvData;
    }
    else
    {
        qDebug()<<"reply error!";
    }
    reply->deleteLater();

    return httpRecvData;
}

