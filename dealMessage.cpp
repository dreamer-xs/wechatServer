#include "dealMessage.h"

//接收消息主要有以下几类
//1. 接收普通消息
//2. 接收事件推送
//3. xxx
DealMessage::DealMessage()
{
}

DealMessage::~DealMessage()
{
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
//      //接收video
//      "<xml><ToUserName><![CDATA[gh_5a42d45d5ea2]]></ToUserName>
//      <FromUserName><![CDATA[oQAcB1CGfUF-KHu-1Hdep23DV9Ok]]></FromUserName>
//      <CreateTime>1554968722</CreateTime>
//      <MsgType><![CDATA[video]]></MsgType>
//      <MediaId><![CDATA[JzI0HtTIA0xU4B9WlEnXOucVLj4OxsYxwBkezw1fUimTMuiuiHjZHsbfGZmEgtEk]]></MediaId>
//      <ThumbMediaId><![CDATA[dv1B00s2zuHFVytNUYypowKfU72fHDl0WHSfhm62CL1HtsglXIno21MTaav-ggS1]]></ThumbMediaId>
//      <MsgId>22261800107117148</MsgId>
//      </xml>"
//
//      //回复video
//      <xml>
//      <ToUserName><![CDATA[toUser]]></ToUserName>
//      <FromUserName><![CDATA[fromUser]]></FromUserName>
//      <CreateTime>12345678</CreateTime>
//      <MsgType><![CDATA[video]]></MsgType>
//      <Video>
//      <MediaId><![CDATA[media_id]]></MediaId>
//      <Title><![CDATA[title]]></Title>
//      <Description><![CDATA[description]]></Description>
//      </Video>
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


void DealMessage::messageJoint(QString httpBody)
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

QString DealMessage::dealVideoMessage(QDomDocument xml)
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

QString DealMessage::dealTextMessage1(QDomDocument xml)
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


QString DealMessage::dealTextMessage(QDomDocument xml)
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

#if 0
    qDebug()<<"-----------获取deviceID并且发送数据--------------";
    UserAuth userDB;
    bool valid = userDB.isValid(fromUserName);
    qDebug()<<"valid: "<<valid;

    if(!valid)
    {
        QString id = "8888";
        userDB.saveToDB(fromUserName, id);
    }
    int socket = userDB.getSocket(fromUserName);
    qDebug()<<"设备socket: " <<socket;

    if(socket > 0)
    {
        QTcpSocket devSocket;
        devSocket.setSocketDescriptor(socket);
        qDebug()<<"SocketState: "<<devSocket.state();

        devSocket.write(content.toUtf8());
    }
    qDebug()<<"--------------------------------------------";

#endif

    /*
    if(content.contains("设备ID") == 0)
    {
        qDebug()<<"绑定设备ID";
    }
    else if(QString::compare(content, "拍照") == 0)
    */
    if(QString::compare(content, "拍照") == 0)
    {
        TokenGet t;
        QString m_token = t.tokenValue();
#if 1
        //简单的QTJson数据构造
        QJsonObject simp_ayjson;
        simp_ayjson.insert("expires_in", 7200);
        simp_ayjson.insert("token", m_token); 
        QJsonDocument document;
        document.setObject(simp_ayjson);

        QByteArray simpbyte_array = document.toJson(QJsonDocument::Compact);
        QString simpjson_str(simpbyte_array);
        qDebug() << simpjson_str;
#endif
        QString url("http://viphk.ngrok.org:10031");
        HttpClient h;
        QString data = h.post(url, simpjson_str);

        qDebug()<<"---------------接收客户端数据-------------------";
        qDebug()<<data;

        if(!data.isEmpty())
        {
            qDebug()<<"------------------------------------------------";

            qDebug()<<"---------------获取mediaID--------------------";
            QString mediaId = getJsonValue(data,"mediaID");
            qDebug()<<"mediaID: "<<mediaId;
            QString m_type = "image";
            return createImage(fromUserName, toUserName, m_type, mediaId);
        }
        else
            return createText(fromUserName, toUserName, msgType, content);
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

QString DealMessage::dealEventMessage(QDomDocument xml)
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

QString DealMessage::createText(QString fromUserName, QString toUserName, QString msgType, QString content)
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
QString DealMessage::createVideo(QString fromUserName, QString toUserName, QString msgType, QString mediaId)
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
QString DealMessage::createImage(QString fromUserName, QString toUserName, QString msgType, QString mediaId)
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

QString DealMessage::dealImageMessage(QDomDocument xml)
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


void DealMessage::messageRecv(QString recvData)
{
    qDebug()<<"------------接收消息并开始分类-------------";

    //分割HTTP头和正文
    QStringList dataList = recvData.split("\r\n\r\n");
    QString head = dataList.at(0);
    QString body = dataList.at(1);


    //解析微信xml
    QDomDocument xml;
    xml.setContent(body);

    //读取根元素<xml>
    QDomElement root = xml.documentElement(); 

    //获取msgtype类型
    QDomElement node = root.firstChildElement("MsgType");
    QString msgType = node.text();

    //类型集合有:
    //文本消息      文本为text
    //图片消息      图片为image
    //语音消息      语音为voice
    //视频消息      视频为video
    //小视频消息    小视频为shortvideo
    //地理位置消息  地理位置为location
    //链接消息      链接为link
    //
    //接收事件推送  消息类型event   -> 继续细分事件类型
    QStringList msgTypeList = {"text","image","voice","video","shortvideo","location","link"};
    /*
     * text  = 0
     * image = 1
     * voice = 2
     * video = 3
     * shortvideo = 4
     * location = 5
     * link = 6
     * event = 7
     */

    QString httpBody;
    switch(msgTypeList.indexOf(msgType))
    {
        case 0:
            qDebug()<<"msgType: text";
            httpBody = dealTextMessage(xml);
            break;
        case 1:
            qDebug()<<"msgType: image";
            httpBody = dealImageMessage(xml);
            break;
        case 2:
            qDebug()<<"msgType: voice";
            httpBody = dealTextMessage1(xml);
            break;
        case 3:
            qDebug()<<"msgType: video";
            httpBody = dealTextMessage1(xml);
            break;
        case 4:
            qDebug()<<"msgType: shortvideo";
            httpBody = dealTextMessage1(xml);
            break;
        case 5:
            qDebug()<<"msgType: location";
            //httpBody = dealTextMessage1(xml);
            httpBody = "success";
            break;
        case 6:
            qDebug()<<"msgType: link";
            httpBody = dealTextMessage1(xml);
            break;
        case 7:
            qDebug()<<"msgType: event";
            httpBody = dealTextMessage1(xml);
            break;
        default:
            qDebug()<<"msgType: unknow";
            break;
    }

    messageJoint(httpBody);
}

QString DealMessage::getJsonValue(QString data, QString key)
{
    qDebug()<<"--------------解析json-----------------";

    //解析JSON
    QJsonParseError json_error;
    QString value;

    QJsonDocument data_parse_doucment = QJsonDocument::fromJson(data.toUtf8(), &json_error); //QTJSON所有的元素
    if (json_error.error == QJsonParseError::NoError) //检查json是否有错误
    {
        if (data_parse_doucment.isObject())
        {
            QJsonObject obj = data_parse_doucment.object(); //开始解析json对象
            if (obj.contains(key)) //如果包含该key
            {
                QJsonValue t_value = obj.take(key);
                if (t_value.isString())
                {
                    value = t_value.toVariant().toString();
                }
            }
        }
    }
    qDebug() << "value: "<< value;
    return value;
}
