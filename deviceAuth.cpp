#include "deviceAuth.h"

/*
 *通过800端口接收设备端的消息
 * 1. 设备首测注册id入库
 * 2. 设备心跳包
 * 3. 设备交互
 *
 */
DeviceAuth::DeviceAuth()
{
    QString dbName = "wechat.db";
    QString tableName = "device";
    db = new DbOperation(dbName);

    QString sqlCmd;

    //判断表是否存在,如果是第一次登陆则先创建表
    sqlCmd.clear();
    //sqlCmd.append(QString("SELECT COUNT(*) FROM sqlite_master where type='table' and name='%1'").arg(tableName));
    sqlCmd.append("SELECT COUNT(*) FROM sqlite_master where type='table' and name='device'");
    QList<QString> dataList;
    dataList = db->sqlQuery(sqlCmd);

    //如果该表不存在则创建表(首次创建)
    if(dataList.at(0).toInt() == 0)
    {
        sqlCmd.clear();
        sqlCmd.append("create table device(id string, status int, socket int)");
        db->createTable(sqlCmd);
    }
    else
    {
        qDebug()<<"表已存在，无需创建";
    }
}

DeviceAuth::~DeviceAuth()
{
    if(db)
        delete db;
}

//查询设备是否已经入库(每次设备连上服务器都会发送ID, 故判断一下设备是否已经入库)
bool DeviceAuth::isValid(QString deviceId)
{
    QString sqlCmd;
    QList<QString> dataList;
    sqlCmd.clear();
    sqlCmd.append(QString("SELECT * FROM device where id='%1'").arg(deviceId));
    dataList = db->sqlQuery(sqlCmd);
    if(dataList.isEmpty())
    {
        qDebug()<<"获取数据为空，设备未入库";
        return false;
    }
    QStringList strList = dataList.at(0).split(",");
    QString id = strList.at(0);
    QString status = strList.at(1);
    QString socket = strList.at(2);
    qDebug()<<"设备已入库"<<" id:"<<id<<" status:"<<status<<" socket:"<<socket;
    return true;
}

//新设备ID入库,以及状态,socket刷新
bool DeviceAuth::saveToDB(QString id, int status, int socket)
{
    QString sqlCmd;
    //先清除失效的数据
    sqlCmd.clear();
    sqlCmd.append(QString("DELETE FROM device where id='%1'").arg(id));
    db->delData(sqlCmd);

    //插入新获取的数据
    sqlCmd.clear();
    sqlCmd.append("INSERT INTO device VALUES(?,?,?)");
    QString data = QString("%1,%2,%3").arg(id).arg(status).arg(socket);
    QList<QString> dataList;
    dataList.append(data);

    return db->insertData(sqlCmd, dataList);
}

//获取设备socket进行通讯
int DeviceAuth::getSocket(QString deviceId)
{
    QString sqlCmd;
    QList<QString> dataList;
    sqlCmd.clear();
    sqlCmd.append(QString("SELECT * FROM device where id='%1'").arg(deviceId));
    dataList = db->sqlQuery(sqlCmd);
    if(dataList.isEmpty())
    {
        qDebug()<<"获取数据为空，设备未入库";
        return -1;
    }
    QStringList strList = dataList.at(0).split(",");
    QString id = strList.at(0);
    QString status = strList.at(1);
    QString socket = strList.at(2);
    qDebug()<<"设备已入库"<<" id:"<<id<<" status:"<<status<<" socket:"<<socket;

    //若设备不在线，则返回-1
    if(status == 0)
        return -1;

    return socket.toInt();
}

void DeviceAuth::run()
{
    
}
