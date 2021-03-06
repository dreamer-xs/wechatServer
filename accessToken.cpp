#include "accessToken.h"

AccessToken::AccessToken()
{
    //tokenFilePath = "./token.txt";
    tokenValid = false;
    accessTokenValue.clear();
    tokenGetTime = 0;

    QString dbName = "wechat.db";
    QString tableName = "accessToken";
    db = new DbOperation(dbName);

    QString sqlCmd;

    //判断表是否存在,如果是第一次登陆则先创建表
    sqlCmd.clear();
    //sqlCmd.append(QString("SELECT COUNT(*) FROM sqlite_master where type='table' and name='%1'").arg(tableName));
    sqlCmd.append("SELECT COUNT(*) FROM sqlite_master where type='table' and name='accessToken'");
    QList<QString> dataList;
    dataList = db->sqlQuery(sqlCmd);

    //如果该表不存在则创建表(首次创建)
    if(dataList.at(0).toInt() == 0)
    {
        sqlCmd.clear();
        sqlCmd.append("create table accessToken(updateTime int , token string)");
        db->createTable(sqlCmd);
    }
    else
    {
        qDebug()<<"表已存在，无需创建";
    }
}

AccessToken::~AccessToken()
{
    if(tokenClient)
        delete tokenClient;

    if(db)
        delete db;
}

QString AccessToken::tokenGetFromServer()
{
    qDebug()<<"-----------从服务器获取token-----------------";
    //获取access token
    QString APPID = "wx196f2be419727875";
    QString secret = "59eb18c0835bb20fa1d4dd1068ffde3a";
    QString url = QString("https://api.weixin.qq.com/cgi-bin/token?grant_type=client_credential&appid=%1&secret=%2").arg(APPID).arg(secret);     //获取地址

    return tokenClient->get(url);
}

//外部获取token接口
QString AccessToken::tokenValue()
{
    if(tokenValid)
        return accessTokenValue;
    else
    {
        qDebug()<<"token无效";
        return NULL;
    }
}

//一直运行使能token永久有效
void AccessToken::run()
{
    qDebug()<<"-----------token线程启动-----------------";
    tokenClient = new HttpClient; 

    while(1)
    {
        //判断本地token是否有效
        //tokenGetFromCache();
        tokenGetFromDB();
        if(tokenValid == false)
        {
            QString httpData = tokenGetFromServer();
            accessTokenValue = dealHttpData(httpData);
            //tokenGetTime = tokenSaveToCache(accessTokenValue);
            tokenGetTime = tokenSaveToDB(accessTokenValue);
        }

        //qDebug()<<"DB connection:"<<DbOperation::getConnection();
        sleep(60);
        //qDebug()<<"获取tokenAPI: "<<tokenValue();
    }
    qDebug()<<"-----------token线程结束-----------------";
}

//从httpbody解析token
QString AccessToken::dealHttpData(QString data)
{
#if 0
	//简单的QTJson数据构造
	QJsonObject simp_ayjson;
	simp_ayjson.insert("expires_in", 7200);
	//simp_ayjson.insert("access_token", QString::fromLocal8Bit("123你好"));
	simp_ayjson.insert("access_token", "20_B24Lh5RmwRamcWx-BjvO0Bysf1ZGo41rTVl7DewO9wd-hx2NPuD6dQeVm8z2hIQiLJOeOYEqhyTFbUgm9zBLCwzotcwHcaDBZYZmaM0g-ojG1RyOFDh702JjuuJgmYTgCBTpySNwsK6ofz-7GUDdABALZI"); 
	QJsonDocument document;
	document.setObject(simp_ayjson);

	QByteArray simpbyte_array = document.toJson(QJsonDocument::Compact);
	QString simpjson_str(simpbyte_array);
	qDebug() << simpjson_str;
#endif
    qDebug()<<"--------------解析json,并获取token-----------------";

	//简单的QT解析JSON
	QJsonParseError json_error;

	QString accessToken;
	int expires;

	//QTJSON所有的元素
	QJsonDocument data_parse_doucment = QJsonDocument::fromJson(data.toUtf8(), &json_error);
	//检查json是否有错误
	if (json_error.error == QJsonParseError::NoError)
	{
		if (data_parse_doucment.isObject())
		{
			//开始解析json对象
			QJsonObject obj = data_parse_doucment.object();
			//如果包含access_token
			if (obj.contains("access_token"))
			{
				//得到access_token
				QJsonValue token_value = obj.take("access_token");
				if (token_value.isString())
				{
					//转换token
					accessToken = token_value.toVariant().toString();
				}
			}
			if (obj.contains("expires_in"))
			{
				QJsonValue expires_value = obj.take("expires_in");
				if (expires_value.isDouble())
				{
					expires = expires_value.toVariant().toInt();
				}
			}
		}
	}

	//qDebug() << "access_token: "<< accessToken;
	//qDebug() << "expires_in: "<< expires;

    tokenValid = true;
    return accessToken;
}

void AccessToken::tokenGetFromDB()
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
        tokenValid = false;
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
        tokenValid = true;
    }
    else
    {
        qDebug()<<"token无效";
        accessTokenValue.clear();
        tokenGetTime = 0;
        tokenValid = false;
    }
}
void AccessToken::tokenGetFromCache()
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
        tokenValid = false;

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
 
    tokenGetTime = time.toInt();
    accessTokenValue = token;

    //判断是否token是否过期,有效期为7200秒
    if((timeT-7000) < time.toInt())
    {
        qDebug()<<"token有效";
        tokenValid = true;
    }
    else
    {
        qDebug()<<"token无效";
        accessTokenValue.clear();
        tokenGetTime = 0;
        tokenValid = false;
    }
}

int AccessToken::tokenSaveToDB(QString token)
{
    qDebug()<<"--------将access_token存储到数据库----------";
    //获取当前时间戳
	QDateTime time = QDateTime::currentDateTime();   //获取当前时间  
	int timeT = time.toTime_t();   //将当前时间转为时间戳 

    QString sqlCmd;
    //先清除失效的数据
    sqlCmd.clear();
    sqlCmd.append("DELETE FROM accessToken");
    //db->sqlExec(sqlCmd);
    db->delData(sqlCmd);

    //插入新获取的数据
    sqlCmd.clear();
    sqlCmd.append("INSERT INTO accessToken VALUES(?,?)");
    QString data = QString("%1,%2").arg(timeT).arg(token);
    QList<QString> dataList;
    dataList.append(data);

    db->insertData(sqlCmd, dataList);

	//qDebug()<<"token: "<<token;
	//qDebug()<<"time: "<<timeT;

    tokenValid = true;
    return timeT;
}
//将token缓存至本地
int AccessToken::tokenSaveToCache(QString token)
{
    qDebug()<<"--------将access_token存储到本地-----------";
    //获取当前时间戳
	QDateTime time = QDateTime::currentDateTime();   //获取当前时间  
	int timeT = time.toTime_t();   //将当前时间转为时间戳 

    QFile file(tokenFilePath);
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate))
    {
        qDebug()<<"Failed to open: "<<tokenFilePath;
        tokenValid = false;
        accessTokenValue.clear();
		return 0;
    }

	QTextStream stream(&file);
	stream.seek(file.size());

	QString qs;
	qs.append("access_token=");
	qs.append(token);
	qs.append("\n");
	qs.append("time=");
	qs.append(QString::number(timeT));
    //qDebug()<<qs;
	//qs.remove("\n");
	//stream << qs << "\n";
	stream << qs;

	file.close();
    tokenValid = true;
    return timeT;
}

