#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>    //加载网络请求头文件
#include <QNetworkReply>
#include <QNetworkRequest>      //加载发送请求头文件
#include <QDebug>
#include <QEventLoop>
#include <unistd.h>

class HttpClient : public QObject
{
    Q_OBJECT

public:
    HttpClient();
    ~HttpClient();
    QString get(QString);        //初始化函数

signals:
    //void httpDataReceived(QString);

private slots:
    //void oneProcessFinished(QNetworkReply*);
    //void dataHandle(QString);

private:
    QNetworkAccessManager *manager;      //定义
    QString url;
    QString httpRecvData;
};

#endif // HTTPCLIENT_H
