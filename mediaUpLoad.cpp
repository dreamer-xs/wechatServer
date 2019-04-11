#include "mediaUpLoad.h"

MediaUpLoad::MediaUpLoad(QString m_token, QString m_type, QString m_path)
{
    token = m_token;
    type = m_type;
    mediaPath = m_path;
}

MediaUpLoad::MediaUpLoad()
{
    token = "20_h9nU3ca98TucfYwPLsHLS4qOe4uf5xsm9hAjnd6Hf7L1QuC7jvpJP2SjLDiNNg_gFvd9WpmWieiiCANWK6s4qhb2wvfqWSoUTlAsq0CfLGth2P7zOeY1SKq5CmsICKdAIAVPX";
    type = "image";
    mediaPath = "test.jpg";
}

MediaUpLoad::~MediaUpLoad()
{

}

void MediaUpLoad::run()
{
	QString data = uploadMedia(token, type, mediaPath);
    //qDebug()<<"data: "<<data;
	dealData(data);
}

//图片上传返回
/*
{
    "type": "image", 
    "media_id": "j_OvVHrUc1OT9icQLbzm1lpwDrZOrsSD7cJGg-0mYGMosPz3hNs2qoIgC3dAm3HA", 
    "created_at": 1554876992
}
*/

void MediaUpLoad::dealData(QString data)
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

    qDebug()<<"--------------解析json-----------------";

	//简单的QT解析JSON
	QJsonParseError json_error;

	QString type, mediaId;

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
			if (obj.contains("type"))
			{
				QJsonValue t_value = obj.take("type");
				if (t_value.isString())
				{
					type = t_value.toVariant().toString();
				}
			}
			if (obj.contains("media_id"))
			{
				QJsonValue m_value = obj.take("media_id");
				if (m_value.isString())
				{
					mediaId = m_value.toVariant().toString();
				}
			}
		}
	}

	//qDebug() << "type: "<< type;
	//qDebug() << "media_id: "<< mediaId;

    connect(this, SIGNAL(mediaUpLoadFinished(QString,QString)), this, SLOT(mediaGet(QString, QString)));
    emit mediaUpLoadFinished(type, mediaId);
}

void MediaUpLoad::mediaGet(QString type, QString mediaId)
{
	qDebug() << "type: "<< type;
	qDebug() << "media_id: "<< mediaId;
}

//type  是  媒体文件类型，分别有图片（image）、语音（voice）、视频（video）和缩略图（thumb）
QString MediaUpLoad::uploadMedia(QString token, QString type, QString path)
{
    qDebug()<<"-----------------上传media素材------------------";
	QString url = QString("curl -F media=@%1 \"https://api.weixin.qq.com/cgi-bin/media/upload?access_token=%2&type=%3\"").arg(path).arg(token).arg(type);
	//qDebug()<<url;

	QProcess process;
	process.start(url);
	process.waitForFinished();
	QByteArray output = process.readAllStandardOutput();

	QString data = output;

	//qDebug()<<data;
    return data;
    
}
