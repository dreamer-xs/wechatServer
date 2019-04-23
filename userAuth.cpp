#include "userAuth.h"

/*
 *通过800端口接收设备端的消息
 * 1. 设备首测注册id入库
 * 2. 设备心跳包
 * 3. 设备交互
 *
 */
UserAuth::UserAuth()
{
    QString dbName = "wechat.db";
    QString tableName = "user";
    db = new DbOperation(dbName);

    QString sqlCmd;

    //判断表是否存在,如果是第一次登陆则先创建表
    sqlCmd.clear();
    //sqlCmd.append(QString("SELECT COUNT(*) FROM sqlite_master where type='table' and name='%1'").arg(tableName));
    sqlCmd.append("SELECT COUNT(*) FROM sqlite_master where type='table' and name='user'");
    QList<QString> dataList;
    dataList = db->sqlQuery(sqlCmd);

    //如果该表不存在则创建表(首次创建)
    if(dataList.at(0).toInt() == 0)
    {
        sqlCmd.clear();
        sqlCmd.append("create table user(userId string, deviceId string)");
        db->createTable(sqlCmd);
    }
    else
    {
        qDebug()<<"表已存在，无需创建";
    }
}

UserAuth::~UserAuth()
{
    if(db)
        delete db;
}

//查询用户是否绑定设备,只有已绑定设备的用户才有操作设备的权限
bool UserAuth::isValid(QString userId)
{
    QString sqlCmd;
    QList<QString> dataList;
    sqlCmd.clear();
    sqlCmd.append(QString("SELECT * FROM user where userId='%1'").arg(userId));
    dataList = db->sqlQuery(sqlCmd);
    if(dataList.isEmpty())
    {
        qDebug()<<"获取数据为空，设备未入库";
        return false;
    }
    QStringList strList = dataList.at(0).split(",");
    QString id = strList.at(0);
    QString deviceId = strList.at(1);
    qDebug()<<"设备已入库"<<" userId:"<<id<<" deviceId:"<<deviceId;
    return true;
}

bool UserAuth::delFromDB(QString userId)
{
    QString sqlCmd;
    //先清除失效的数据
    sqlCmd.clear();
    sqlCmd.append(QString("DELETE FROM user where userId='%1'").arg(userId));
    return db->delData(sqlCmd);
}

//新设备ID入库,以及状态,socket刷新
bool UserAuth::saveToDB(QString userId, QString deviceId)
{
    QString sqlCmd;
    //先清除失效的数据
    sqlCmd.clear();
    //sqlCmd.append(QString("DELETE FROM user where userId='%1'").arg(userId));
    sqlCmd.append(QString("DELETE FROM user"));
    db->delData(sqlCmd);

    //插入新获取的数据
    sqlCmd.clear();
    sqlCmd.append("INSERT INTO user VALUES(?,?)");
    QString data = QString("%1,%2").arg(userId).arg(deviceId);
    QList<QString> dataList;
    dataList.append(data);

    return db->insertData(sqlCmd, dataList);
}

//获取设备socket进行通讯
int UserAuth::getSocket(QString userId)
{
    QString sqlCmd;
    QList<QString> dataList;
    sqlCmd.clear();
    //SELECT d.socket FROM device AS d INNER JOIN user AS u WHERE d.id = u.deviceId and u.userId='张三';
    sqlCmd.append(QString("SELECT d.socket,d.status FROM device AS d INNER JOIN user AS u WHERE d.id = u.deviceId and u.userId='%1';").arg(userId));
    dataList = db->sqlQuery(sqlCmd);
    if(dataList.isEmpty())
    {
        qDebug()<<"获取数据为空，设备未入库";
        return -1;
    }
    QStringList strList = dataList.at(0).split(",");
    QString socket = strList.at(0);
    QString status = strList.at(1);
    qDebug()<<"该设备已入库"<<" id:"<<userId<<" status:"<<status<<" socket:"<<socket;

    //若设备不在线，则返回-1
    if(status == 0)
        return -1;

    return socket.toInt();
}

void UserAuth::run()
{
    
}

