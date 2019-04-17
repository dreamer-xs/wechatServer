#include "dealDevice.h"

DealDevice::DealDevice()
{
}

DealDevice::~DealDevice()
{

}

void DealDevice::messageJoint(QString httpBody)
{
    qDebug()<<"-------------组合并发送数据--------------";

    QString httpHead = "httpHead/1.1 200 OK\r\n";
    httpHead += "Server: nginx\r\n";
    httpHead += "Content-Type: application/octet-stream;charset=utf-8\r\n";
    httpHead += "Connection: keep-alive\r\n";
    httpHead += QString("Content-Length: %1\r\n\r\n").arg(httpBody.length());

    QString httpPacket;
    httpPacket.append(httpHead);
    httpPacket.append(httpBody);

    qDebug()<<httpPacket;
    qDebug()<<"------------------------------------------";

    emit messageReady(httpPacket);
}

QString DealDevice::dealVideoMessage(QDomDocument xml)
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
    qDebug()<<msgType;

    msgType.clear();
    msgType = "video";
    QString mediaId = "NWkZEpSsgyAJfVpcodJcrkbaT_RqiS4Xfs9LgdEA2UDmwuIKhpG1CrcibXXAAOmg";

    return createVideo(fromUserName, toUserName, msgType, mediaId);
}

QString DealDevice::dealTextMessage1(QDomDocument xml)
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

    qDebug()<<"--------------修改后数据--------------------";
    qDebug()<<xmlString;
    qDebug()<<"--------------------------------------------";

    return xmlString;
}


QString DealDevice::dealTextMessage(QDomDocument xml)
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

    if(QString::compare(content, "拍照") == 0)
    {
        TokenGet t;
        QString m_token = t.tokenValue();
        QString m_type = "image";
        //QString m_mediaPath = "picture/test.jpg";
        QString m_mediaPath = "picture/kobe.jpg";

        MediaUpLoad m(m_token, m_type, m_mediaPath);
        QString mediaId = m.mediaIdGet();

        return createImage(fromUserName, toUserName, m_type, mediaId);
    }
    else if(QString::compare(content, "监控") == 0)
    {
        msgType.clear();
        msgType = "video";
        QString mediaId = "NWkZEpSsgyAJfVpcodJcrkbaT_RqiS4Xfs9LgdEA2UDmwuIKhpG1CrcibXXAAOmg";
        return createVideo(fromUserName, toUserName, msgType, mediaId);
    }

    else
        return createText(fromUserName, toUserName, msgType, content);
}

QString DealDevice::dealEventMessage(QDomDocument xml)
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

    return xmlString;
}

QString DealDevice::createText(QString fromUserName, QString toUserName, QString msgType, QString content)
{

    QDomDocument xmlRoot("");
    QDomElement xmlNode= xmlRoot.createElement("xml");
    xmlRoot.appendChild(xmlNode);
     
    //创建一个元素标签节点
    QDomElement node = xmlRoot.createElement("FromUserName");
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
    //QDomText time  = xmlRoot.createTextNode(createTime);
    QDomText time  = xmlRoot.createTextNode(0);
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

    qDebug()<<"--------------修改后数据--------------------";
    qDebug()<<xmlString;
    qDebug()<<"--------------------------------------------";

    return xmlString;

}
QString DealDevice::createVideo(QString fromUserName, QString toUserName, QString msgType, QString mediaId)
{
    QDomDocument xmlRoot("");
    QDomElement xmlNode= xmlRoot.createElement("xml");
    xmlRoot.appendChild(xmlNode);
     
    //创建一个元素标签节点
    QDomElement node = xmlRoot.createElement("FromUserName");
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
    //QDomText time  = xmlRoot.createTextNode(createTime);
    QDomText time  = xmlRoot.createTextNode(0);
    node.appendChild(time);

    node = xmlRoot.createElement("MsgType");
    xmlNode.appendChild(node);
    t = xmlRoot.createCDATASection(msgType);
    node.appendChild(t);

    node = xmlRoot.createElement("Video");
    xmlNode.appendChild(node);

    QDomElement subNode = xmlRoot.createElement("MediaId");
    node.appendChild(subNode);

    t = xmlRoot.createCDATASection(mediaId);
    subNode.appendChild(t);

    subNode = xmlRoot.createElement("Title");
    node.appendChild(subNode);

    QString title = "监控";
    t = xmlRoot.createCDATASection(title);
    subNode.appendChild(t);

    subNode = xmlRoot.createElement("Description");
    node.appendChild(subNode);

    QString description = "办公室一角";
    t = xmlRoot.createCDATASection(description);
    subNode.appendChild(t);

    QString xmlString = xmlRoot.toString();

    qDebug()<<"--------------修改后数据--------------------";
    qDebug()<<xmlString;
    qDebug()<<"--------------------------------------------";

    return xmlString;
}
QString DealDevice::createImage(QString fromUserName, QString toUserName, QString msgType, QString mediaId)
{
    QDomDocument xmlRoot("");
    QDomElement xmlNode= xmlRoot.createElement("xml");
    xmlRoot.appendChild(xmlNode);
     
    //创建一个元素标签节点
    QDomElement node = xmlRoot.createElement("FromUserName");
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
    //QDomText time  = xmlRoot.createTextNode(createTime);
    QDomText time  = xmlRoot.createTextNode(0);
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

    qDebug()<<"--------------修改后数据--------------------";
    qDebug()<<xmlString;
    qDebug()<<"--------------------------------------------";

    return xmlString;
}

QString DealDevice::dealImageMessage(QDomDocument xml)
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


    return createImage(fromUserName, toUserName, msgType, mediaId);
}


void DealDevice::messageRecv(QString recvData)
{
    qDebug()<<"------------接收设备消息--------------";

    qDebug()<<recvData;

    emit messageReady(recvData);

    //messageJoint(httpBody);
}
