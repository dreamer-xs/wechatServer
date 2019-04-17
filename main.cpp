#include "tcpserver.h"
#include "httpClient.h"
#include "mediaUpLoad.h"
#include "accessToken.h"
#include "tokenGet.h"
#include "dealMessage.h"
#include "dbOperation.h"

#include <QCoreApplication>
#include <QDebug>
#include <QTime>
#include <QMutex>

//全局日志文件路径
#define LOGPATH   "./log.txt"
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

int main(int argc, char **argv)
{
    // 安装消息处理程序
    //qInstallMessageHandler(myMessageOutput);
    qInfo("--------main process begion-------");

    QCoreApplication a(argc, argv);

    // 打印信息级别
    //qDebug("This is a debug message.");
    //qInfo("This is a info message.");
    //qWarning("This is a warning message.");
    //qCritical("This is a critical message.");

#if 1

    //token守护线程,一直确保token是有效的
    AccessToken  token;
    token.start();

    TcpServer wechatServer;
    wechatServer.listen(QHostAddress::Any,80);

    TcpServer deviceServer;
    deviceServer.listen(QHostAddress::Any,800);
# else

    HttpServer h;

#endif


    return a.exec();//开始主线程事件循环

    return 0;
}

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // 加锁
    static QMutex mutex;
    mutex.lock();

    QByteArray localMsg = msg.toLocal8Bit();

    QString strMsg("");
    switch(type)
    {
    case QtDebugMsg:
        strMsg = QString("Debug:");
        break;
    case QtInfoMsg:
        strMsg = QString("Info:");
        break;
    case QtWarningMsg:
        strMsg = QString("Warning:");
        break;
    case QtCriticalMsg:
        strMsg = QString("Critical:");
        break;
    case QtFatalMsg:
        strMsg = QString("Fatal:");
        break;
    }

    // 设置输出信息格式
    QString strDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    //QString strMessage = QString("Message:%1 File:%2  Line:%3  Function:%4  DateTime:%5").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function).arg(strDateTime);
    QString strMessage = QString("DateTime:%1 File:%2  Function:%3  Line:%4 Type:%5 Message:%6").arg(strDateTime).arg(context.file).arg(context.function).arg(context.line).arg(strMsg).arg(localMsg.constData());

    // 输出信息至文件中（读写、追加形式）
    QFile file(LOGPATH);
    file.open(QIODevice::ReadWrite | QIODevice::Append);
    QTextStream stream(&file);
    stream << strMessage << "\r\n";
    file.flush();
    file.close();

    // 解锁
    mutex.unlock();
}

