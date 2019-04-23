#include "tokenGet.h"

TokenGet::TokenGet()
{
    accessTokenValue.clear();
    tokenGetTime = 0;

    QString dbName = "wechat.db";
    QString tableName = "accessToken";
    db = new DbOperation(dbName);

}

TokenGet::~TokenGet()
{

}

//外部获取token接口
QString TokenGet::tokenValue()
{
    qDebug()<<"--------获取access_token接口-----------";
	QDateTime currTime = QDateTime::currentDateTime();   //获取当前时间  
	int timeT = currTime.toTime_t();   //将当前时间转为时间戳 

    //判断是否token是否过期,有效期为7200秒
    if((timeT-7000) < tokenGetTime && accessTokenValue.isEmpty() != true)
    {
        qDebug()<<"token有效";
    }
    else
    {
        qDebug()<<"token无效";
        tokenGetFromDB();
    }

    qDebug()<<accessTokenValue;
    return accessTokenValue;
}

void TokenGet::tokenGetFromDB()
{
    qDebug()<<"--------获取数据库中 access_token-----------";

	QDateTime currTime = QDateTime::currentDateTime();   //获取当前时间  
	int timeT = currTime.toTime_t();   //将当前时间转为时间戳 

    QString sqlCmd;
    QList<QString> dataList;
    sqlCmd.clear();
    sqlCmd.append("SELECT * FROM accessToken");
    dataList = db->sqlQuery(sqlCmd);
    if(dataList.isEmpty())
    {
        qDebug()<<"获取数据为空，token无效";
        accessTokenValue.clear();
        tokenGetTime = 0;
        return;
    }


	QStringList strList = dataList.at(0).split(",");
	QString time = strList.at(0);
	QString token = strList.at(1);

	//qDebug()<<"token: "<<token;
	//qDebug()<<"time: "<<time;
 
    tokenGetTime = time.toInt();
    accessTokenValue = token;

    //判断是否token是否过期,有效期为7200秒
    if((timeT-7000) < time.toInt())
    {
        qDebug()<<"token有效";
    }
    else
    {
        qDebug()<<"token无效";
        accessTokenValue.clear();
        tokenGetTime = 0;
    }
}


