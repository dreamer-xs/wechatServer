#include "httpServer.h"
#include "QCoreApplication"

Server::Server(QObject *parent) : QObject(parent)
{
    m_piserver = new QTcpServer;
    m_pisocket = new QTcpSocket;
    StartListen(80);
}

void Server::StartListen(int nPort)
{
    if(m_piserver->listen(QHostAddress::Any,nPort))
        qDebug() << "listen ok";
    else
        qDebug() << "listen err";
    connect(m_piserver,SIGNAL(newConnection()),this,SLOT(newClientConnect()));
}

void Server::newClientConnect()
{
    qDebug() << "new client connect";
    m_pisocket = m_piserver->nextPendingConnection();
    connect(m_pisocket,SIGNAL(readyRead()),this,SLOT(readMessage()));
    connect(m_pisocket,SIGNAL(disconnected()),this,SLOT(disConnect()));
}



//      接收格式
//      "POST /?signature=74ababdbd917619d6bddb1e1799d901c8e8a923f&timestamp=1554721572&nonce=1039027534&openid=oQAcB1CGfUF-KHu-1Hdep23DV9Ok HTTP/1.1
//      User-Agent: Mozilla/4.0
//      Accept: */*
//      Host: 172.96.214.25
//      Pragma: no-cache
//      Content-Length: 283
//      Content-Type: text/xml
//      
//      <xml><ToUserName><![CDATA[gh_5a42d45d5ea2]]></ToUserName> 公众号
//      <FromUserName><![CDATA[oQAcB1CGfUF-KHu-1Hdep23DV9Ok]]></FromUserName> 粉丝号
//      <CreateTime>1554721572</CreateTime> 粉丝发送该消息的具体时间
//      <MsgType><![CDATA[text]]></MsgType> 用于标记该xml 是文本消息
//      <Content><![CDATA[你好吗？]]></Content>
//      <MsgId>22258263273163261</MsgId> 记录识别该消息的一个标记数值
//      </xml>"
//      
//      
//      
//      回复格式
//      <xml>
//      <ToUserName><![CDATA[粉丝号]]></ToUserName>
//      <FromUserName><![CDATA[公众号]]></FromUserName>
//      <CreateTime>1460541339</CreateTime>
//      <MsgType><![CDATA[text]]></MsgType>
//      <Content><![CDATA[test]]></Content>
//      </xml>

void Server::sendMessage(QString httpBody)
//void Server::sendMessage(QString body)
{
    /*
    QString httpBody = "<xml>";
    httpBody += "<ToUserName><![CDATA[oQAcB1CGfUF-KHu-1Hdep23DV9Ok]]></ToUserName>\n";
    httpBody += "<FromUserName><![CDATA[gh_5a42d45d5ea2]]></FromUserName>\n";
    httpBody += "<CreateTime>1460541339</CreateTime>\n";
    httpBody += "<MsgType><![CDATA[text]]></MsgType>\n";
    httpBody += "<Content><![CDATA[帅哥好！]]></Content>\n";
    httpBody += "</xml>";
    */

    QString httpHead = "httpHead/1.1 200 OK\r\n";
    httpHead += "Server: nginx\r\n";
    httpHead += "Content-Type: application/octet-stream;charset=utf-8\r\n";
    httpHead += "Connection: keep-alive\r\n";
    httpHead += QString("Content-Length: %1\r\n\r\n").arg(httpBody.length());

    qDebug()<<"发送消息:";
    qDebug()<<httpHead;
    qDebug()<<httpBody;
    qDebug();

    QByteArray httpData;
    httpData.append(httpHead);
    httpData.append(httpBody);

    //QByteArray httpBodyByte = httpBody.toUtf8();

    m_pisocket->write(httpData);
}

void Server::dealMessage(QString recvData)
{
    qDebug()<<"接收消息:";

    //分割HTTP头和正文
    QStringList dataList = recvData.split("\r\n\r\n");
    QString head = dataList.at(0);
    QString body = dataList.at(1);

    qDebug()<<"---------------原始接收数据----------------------";
    qDebug()<<body;
    qDebug()<<"-----------------------------------------";

    //解析微信xml
    qDebug()<<"----------------修改后数据-----------------------";
    QDomDocument xml;
    xml.setContent(body);

    //读取根元素<xml>
    QDomElement root = xml.documentElement(); 
   
    QStringList listSrc = {"ToUserName", "FromUserName"};
    QStringList listTmp = {"FromUserName_tmp", "ToUserName_tmp"};
    QStringList listDst = {"FromUserName", "ToUserName"};
    for(int i=0;i<listSrc.size();i++)
    {
        QDomElement node = root.firstChildElement(listSrc.at(i));
        node.setTagName(listTmp.at(i));
    }
    for(int i=0;i<listSrc.size();i++)
    {
        QDomElement node = root.firstChildElement(listTmp.at(i));
        node.setTagName(listDst.at(i));
    }
    qDebug()<<xml.toString();

    //将修改后的xml转成QString
    QString httpBody(xml.toString());

    qDebug()<<"-----------------------------------------";
#if 0
    QMap<QString, QString> xmlMap;
    xmlMap["ToUserName"] = "1";
    xmlMap["FromUserName"] = "2";
    xmlMap["CreateTime"] = "3";
    xmlMap["MsgType"] = "4";
    xmlMap["Content"] = "5";
    xmlMap["MsgId"] = "6";

    QMap<QString, QString>::const_iterator i = xmlMap.constBegin();
    while (i != xmlMap.constEnd()) 
    {
        QDomElement node = root.firstChildElement(i.key());
        xmlMap.insert(i.key(),node.text());
        qDebug()<< i.key() << ": " << i.value();
        ++i;
    }

    QString httpBody = "<xml>";
    httpBody += QString("<ToUserName><![CDATA[%1]]></ToUserName>\n").arg(xmlMap["FromUserName"]);
    httpBody += QString("<FromUserName><![CDATA[%1]]></FromUserName>\n").arg(xmlMap["ToUserName"]);
    httpBody += QString("<CreateTime>%1</CreateTime>\n").arg(xmlMap["CreateTime"]);
    httpBody += QString("<MsgType><![CDATA[%1]]></MsgType>\n").arg(xmlMap["MsgType"]);
    httpBody += QString("<Content><![CDATA[%1]]></Content>\n").arg(xmlMap["Content"]);
    httpBody += "</xml>";
#endif

    qDebug()<<httpBody;

    /*
    for(int i=0; i<keyList.size(); i++)
    {
        QDomElement node = root.firstChildElement(keyList.at(i));
        valueList<< node.text();
        //qDebug()<<keyList.at(i)<<":"<<node.text();
        qDebug()<<keyList.at(i)<<":"<<valueList.at(i);
    }
    */


    qDebug()<<"----------------发送数据----------------------";

    //sendMessage(body);
    sendMessage(httpBody);

    /*
    QDomDocument doc("MyML");
    QDomElement root = doc.createElement("MyML");
    doc.appendChild(root);
     
    QDomElement tag = doc.createElement("Greeting");
    root.appendChild(tag);
     
    QDomText t = doc.createTextNode("Hello World");
    tag.appendChild(t);
     
    QString xml = doc.toString();
    qDebug()<<xml;
    */
}

void Server::readMessage()
{
    //qDebug() << "read client message";
    QByteArray buf;
    buf = m_pisocket->readAll();

    QString recvData(buf);
    dealMessage(recvData);



}

void Server::disConnect()
{
    qDebug() << "client disconnect";
}


