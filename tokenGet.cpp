#include "tokenGet.h"

TokenGet::TokenGet()
{
    tokenFilePath = "./token.txt";
    accessTokenValue.clear();
    tokenGetTime = 0;
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
        tokenGetFromCache();
    }

    qDebug()<<accessTokenValue;
    return accessTokenValue;
}

void TokenGet::tokenGetFromCache()
{
    qDebug()<<"--------获取本地缓存 access_token-----------";

	QDateTime currTime = QDateTime::currentDateTime();   //获取当前时间  
	int timeT = currTime.toTime_t();   //将当前时间转为时间戳 

    QFile file(tokenFilePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"Failed to open: "<<tokenFilePath;

        accessTokenValue.clear();
        tokenGetTime = 0;
		return;
    }

	QStringList strList;
	QString token;
	QString time;
	while (!file.atEnd())
	{
		QByteArray line = file.readLine();
		QString str(line);
        str = str.trimmed();      //去掉行尾换行符
		//qDebug() << str;

		strList = str.split("=");
		if(QString::compare(strList.at(0), "access_token") == 0)
			token = strList.at(1);
		if(QString::compare(strList.at(0), "time") == 0)
			time = strList.at(1);
	}
	file.close();
	//qDebug()<<"token: "<<token;
	//qDebug()<<"time: "<<time;
 
    //判断是否token是否过期,有效期为7200秒
    if((timeT-7000) < time.toInt())
    {
        qDebug()<<"token有效";
        tokenGetTime = time.toInt();
        accessTokenValue = token;
    }
    else
    {
        qDebug()<<"token无效";
        accessTokenValue.clear();
        tokenGetTime = 0;
    }
}


