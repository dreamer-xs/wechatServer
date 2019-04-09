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
//      //text
//      <xml><ToUserName><![CDATA[gh_5a42d45d5ea2]]></ToUserName> 公众号
//      <FromUserName><![CDATA[oQAcB1CGfUF-KHu-1Hdep23DV9Ok]]></FromUserName> 粉丝号
//      <CreateTime>1554721572</CreateTime> 粉丝发送该消息的具体时间
//      <MsgType><![CDATA[text]]></MsgType> 用于标记该xml 是文本消息
//      <Content><![CDATA[你好吗？]]></Content>
//      <MsgId>22258263273163261</MsgId> 记录识别该消息的一个标记数值
//      </xml>"
//      
//      //回复text
//      <xml>
//      <ToUserName><![CDATA[粉丝号]]></ToUserName>
//      <FromUserName><![CDATA[公众号]]></FromUserName>
//      <CreateTime>1460541339</CreateTime>
//      <MsgType><![CDATA[text]]></MsgType>
//      <Content><![CDATA[test]]></Content>
//      </xml>
//
//      //接收media
//      <xml>
//      <ToUserName><![CDATA[公众号]]></ToUserName>
//      <FromUserName><![CDATA[粉丝号]]></FromUserName>
//      <CreateTime>1460536575</CreateTime>
//      <MsgType><![CDATA[image]]></MsgType>
//      <PicUrl><![CDATA[http://mmbiz.qpic.cn/xxxxxx /0]]></PicUrl>
//      <MsgId>6272956824639273066</MsgId>
//      <MediaId><![CDATA[gyci5a-xxxxx-OL]]></MediaId>
//      </xml>
//
//      //回复media
//      <xml>
//      <ToUserName><![CDATA[粉丝号]]></ToUserName>
//      <FromUserName><![CDATA[公众号]]></FromUserName>
//      <CreateTime>1460536576</CreateTime>
//      <MsgType><![CDATA[image]]></MsgType>
//      <Image>
//      <MediaId><![CDATA[gyci5oxxxxxxv3cOL]]></MediaId>
//      </Image>
//      </xml>
//      
//      //取消关注
//      <xml><ToUserName><![CDATA[gh_5a42d45d5ea2]]></ToUserName>
//      <FromUserName><![CDATA[oQAcB1CGfUF-KHu-1Hdep23DV9Ok]]></FromUserName>
//      <CreateTime>1554804530</CreateTime>
//      <MsgType><![CDATA[event]]></MsgType>
//      <Event><![CDATA[unsubscribe]]></Event>
//      <EventKey><![CDATA[]]></EventKey>
//      </xml>
//
//      //关注
//      <xml><ToUserName><![CDATA[gh_5a42d45d5ea2]]></ToUserName>
//      <FromUserName><![CDATA[oQAcB1CGfUF-KHu-1Hdep23DV9Ok]]></FromUserName>
//      <CreateTime>1554804610</CreateTime>
//      <MsgType><![CDATA[event]]></MsgType>
//      <Event><![CDATA[subscribe]]></Event>
//      <EventKey><![CDATA[]]></EventKey>
//      </xml>

void Server::sendMessage(QString httpBody)
{
    qDebug()<<"----------------发送数据----------------------";

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

    m_pisocket->write(httpData);
}

void Server::dealTextMessage1(QDomDocument xml)
{
    //读取根元素<xml>
    QDomElement root = xml.documentElement(); 

    //获取msgtype类型
    QDomElement node = root.firstChildElement("ToUserName");
    QString toUserName = node.text();
    node = root.firstChildElement("FromUserName");
    QString fromUserName = node.text();
    node = root.firstChildElement("CreateTime");
    QString createTime = node.text();
    node = root.firstChildElement("MsgType");
    QString msgType = node.text();

    qDebug()<<toUserName;
    qDebug()<<fromUserName;
    qDebug()<<createTime;

    QDomDocument xmlRoot("");
    QDomElement xmlNode= xmlRoot.createElement("xml");
    xmlRoot.appendChild(xmlNode);
     
    //创建一个元素标签节点
    node = xmlRoot.createElement("FromUserName");
    xmlNode.appendChild(node);
    //将CDATAS元素加入创建的元素节点 value
    QDomCDATASection t = xmlRoot.createCDATASection(toUserName);
    node.appendChild(t);

    node = xmlRoot.createElement("ToUserName");
    xmlNode.appendChild(node);
    t = xmlRoot.createCDATASection(fromUserName);
    node.appendChild(t);
     
    node = xmlRoot.createElement("CreateTime");
    xmlNode.appendChild(node);
    QDomText time  = xmlRoot.createTextNode(createTime);
    node.appendChild(time);

    node = xmlRoot.createElement("MsgType");
    xmlNode.appendChild(node);
    t = xmlRoot.createCDATASection("text");
    node.appendChild(t);

    node = xmlRoot.createElement("Content");
    xmlNode.appendChild(node);
    t = xmlRoot.createCDATASection("收到！");
    node.appendChild(t);

    QString xmlString = xmlRoot.toString();

    qDebug()<<"----------------修改后数据-----------------------";
    qDebug()<<xmlString;

    sendMessage(xmlString);
}

void Server::dealTextMessage(QDomDocument xml)
{
    //读取根元素<xml>
    QDomElement root = xml.documentElement(); 

    //获取msgtype类型
    QDomElement node = root.firstChildElement("ToUserName");
    QString toUserName = node.text();
    node = root.firstChildElement("FromUserName");
    QString fromUserName = node.text();
    node = root.firstChildElement("CreateTime");
    QString createTime = node.text();
    node = root.firstChildElement("MsgType");
    QString msgType = node.text();
    node = root.firstChildElement("Content");
    QString content = node.text();
    node = root.firstChildElement("MsgId");
    QString msgId = node.text();

    qDebug()<<toUserName;
    qDebug()<<fromUserName;
    qDebug()<<createTime;
    qDebug()<<msgType;
    qDebug()<<content;
    qDebug()<<msgId;

    QDomDocument xmlRoot("");
    QDomElement xmlNode= xmlRoot.createElement("xml");
    xmlRoot.appendChild(xmlNode);
     
    //创建一个元素标签节点
    node = xmlRoot.createElement("FromUserName");
    xmlNode.appendChild(node);
    //将CDATAS元素加入创建的元素节点 value
    QDomCDATASection t = xmlRoot.createCDATASection(toUserName);
    node.appendChild(t);

    node = xmlRoot.createElement("ToUserName");
    xmlNode.appendChild(node);
    t = xmlRoot.createCDATASection(fromUserName);
    node.appendChild(t);
     
    node = xmlRoot.createElement("CreateTime");
    xmlNode.appendChild(node);
    QDomText time  = xmlRoot.createTextNode(createTime);
    node.appendChild(time);

    node = xmlRoot.createElement("MsgType");
    xmlNode.appendChild(node);
    t = xmlRoot.createCDATASection(msgType);
    node.appendChild(t);

    node = xmlRoot.createElement("Content");
    xmlNode.appendChild(node);
    t = xmlRoot.createCDATASection(content);
    node.appendChild(t);

    QString xmlString = xmlRoot.toString();

    qDebug()<<"----------------修改后数据-----------------------";
    qDebug()<<xmlString;

    sendMessage(xmlString);
}

void Server::dealEventMessage(QDomDocument xml)
{
    //读取根元素<xml>
    QDomElement root = xml.documentElement(); 

    //获取msgtype类型
    QDomElement node = root.firstChildElement("ToUserName");
    QString toUserName = node.text();
    node = root.firstChildElement("FromUserName");
    QString fromUserName = node.text();
    node = root.firstChildElement("CreateTime");
    QString createTime = node.text();
    node = root.firstChildElement("MsgType");
    QString msgType = node.text();
    node = root.firstChildElement("Event");
    QString event = node.text();
    node = root.firstChildElement("EventKey");
    QString eventKey = node.text();

    qDebug()<<toUserName;
    qDebug()<<fromUserName;
    qDebug()<<createTime;
    qDebug()<<msgType;
    qDebug()<<event;
    qDebug()<<eventKey;


    QDomDocument xmlRoot("");
    QDomElement xmlNode= xmlRoot.createElement("xml");
    xmlRoot.appendChild(xmlNode);
     
    //创建一个元素标签节点
    node = xmlRoot.createElement("FromUserName");
    xmlNode.appendChild(node);
    //将CDATAS元素加入创建的元素节点 value
    QDomCDATASection t = xmlRoot.createCDATASection(toUserName);
    node.appendChild(t);

    node = xmlRoot.createElement("ToUserName");
    xmlNode.appendChild(node);
    t = xmlRoot.createCDATASection(fromUserName);
    node.appendChild(t);
     
    node = xmlRoot.createElement("CreateTime");
    xmlNode.appendChild(node);
    QDomText time  = xmlRoot.createTextNode(createTime);
    node.appendChild(time);

    node = xmlRoot.createElement("MsgType");
    xmlNode.appendChild(node);
    t = xmlRoot.createCDATASection(msgType);
    node.appendChild(t);

    //关注后回复
    if(QString::compare(event, "subscribe") == 0)
    {
        node = xmlRoot.createElement("Content");
        xmlNode.appendChild(node);
        t = xmlRoot.createCDATASection("老板，您来啦");
        node.appendChild(t);
    }
    else if(QString::compare(event, "unsubscribe") == 0)
    {
        node = xmlRoot.createElement("Content");
        xmlNode.appendChild(node);
        t = xmlRoot.createCDATASection("老板，您走啦");
        node.appendChild(t);
    }

    QString xmlString = xmlRoot.toString();

    qDebug()<<"----------------修改后数据-----------------------";
    qDebug()<<xmlString;

    sendMessage(xmlString);
    
}

void Server::dealImageMessage(QDomDocument xml)
{
    //读取根元素<xml>
    QDomElement root = xml.documentElement(); 

    //获取msgtype类型
    QDomElement node = root.firstChildElement("ToUserName");
    QString toUserName = node.text();
    node = root.firstChildElement("FromUserName");
    QString fromUserName = node.text();
    node = root.firstChildElement("CreateTime");
    QString createTime = node.text();
    node = root.firstChildElement("MsgType");
    QString msgType = node.text();
    node = root.firstChildElement("PicUrl");
    QString picUrl = node.text();
    node = root.firstChildElement("MediaId");
    QString mediaId = node.text();

    qDebug()<<toUserName;
    qDebug()<<fromUserName;
    qDebug()<<createTime;
    qDebug()<<msgType;
    qDebug()<<picUrl;
    qDebug()<<mediaId;

    QDomDocument xmlRoot("");
    QDomElement xmlNode= xmlRoot.createElement("xml");
    xmlRoot.appendChild(xmlNode);
     
    //创建一个元素标签节点
    node = xmlRoot.createElement("FromUserName");
    xmlNode.appendChild(node);
    //将CDATAS元素加入创建的元素节点 value
    QDomCDATASection t = xmlRoot.createCDATASection(toUserName);
    node.appendChild(t);

    node = xmlRoot.createElement("ToUserName");
    xmlNode.appendChild(node);
    t = xmlRoot.createCDATASection(fromUserName);
    node.appendChild(t);
     
    node = xmlRoot.createElement("CreateTime");
    xmlNode.appendChild(node);
    QDomText time  = xmlRoot.createTextNode(createTime);
    node.appendChild(time);

    node = xmlRoot.createElement("MsgType");
    xmlNode.appendChild(node);
    t = xmlRoot.createCDATASection(msgType);
    node.appendChild(t);

    node = xmlRoot.createElement("Image");
    xmlNode.appendChild(node);
    QDomElement subNode = xmlRoot.createElement("MediaId");
    node.appendChild(subNode);
    t = xmlRoot.createCDATASection(mediaId);
    subNode.appendChild(t);

    QString xmlString = xmlRoot.toString();

    qDebug()<<"----------------修改后数据-----------------------";
    qDebug()<<xmlString;

    sendMessage(xmlString);

}

void Server::dealVoiceMessage(QDomDocument xml)
{
    dealTextMessage1(xml);
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
    QDomDocument xml;
    xml.setContent(body);

    //读取根元素<xml>
    QDomElement root = xml.documentElement(); 

    //获取msgtype类型
    QDomElement node = root.firstChildElement("MsgType");
    QString msgType = node.text();

    //类型集合
    QStringList msgTypeList = {"text", "image", "event", "voice"};
    /*
     * text = 0
     * image = 1
     * event = 2
     * voice = 3
     */

    switch(msgTypeList.indexOf(msgType))
    {
        case 0:
            qDebug()<<"msgType: text";
            dealTextMessage(xml);
            break;
        case 1:
            qDebug()<<"msgType: image";
            dealImageMessage(xml);
            break;
        case 2:
            qDebug()<<"msgType: event";
            dealEventMessage(xml);
            break;
        case 3:
            qDebug()<<"msgType: voice";
            dealVoiceMessage(xml);
            break;
        default:
            qDebug()<<"msgType: unknow";
            break;
    }
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


