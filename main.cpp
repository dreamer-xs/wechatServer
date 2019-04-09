#include "httpServer.h"
#include "streamparsexml.h"
#include <QCoreApplication>
#include <QDebug>
#include <QTime>
#include "log.h"

int main(int argc, char **argv)
{
    LOG_INIT;
    StreamParseXml xml;
    xml.writeXml();

    QCoreApplication a(argc, argv);
    Server *piServer = new Server;
    return a.exec();//开始主线程事件循环

    return 0;
}
