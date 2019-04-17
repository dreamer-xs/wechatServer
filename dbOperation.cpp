#include "dbOperation.h"

DbOperation::DbOperation(QString dbName)
//DbOperation::DbOperation()
{
    db = QSqlDatabase::addDatabase("QSQLITE"); //建立并打开数据库
    db.setDatabaseName(dbName);
    if (!db.open())
    {
        qDebug() << "Error: Failed to connect database." << db.lastError();
    }
    else
    {
        qDebug() << "Succeed to connect database: "<< dbName;
    }

    sql_query = QSqlQuery(db);
}

bool DbOperation::createTable(QString sqlCmd)
{
    //创建表格
    //if(!sql_query.exec("create table student(id int primary key, name text, age int)"))
    if(!sql_query.exec(sqlCmd))
    {
        qDebug() << "Error: Fail to create table."<< sql_query.lastError();
    }
    else
    {
        qDebug() << "Table created!";
    }
    return true;
}

bool DbOperation::insertData(QString sqlCmd, QList<QString> dataList)
{
    sql_query.exec("PRAGMA synchronous = OFF;"); //关闭写同步执行

    //qDebug()<<"关闭写同步执行, 显式开启事务执行";
    sql_query.exec("begin"); //显式开启事务执行
    //QSqlDatabase::database().transaction();

    //qDebug()<<"insert sql: " <<sqlCmd;
    //sql_query.prepare("INSERT INTO data VALUES(?,?)");
    sql_query.prepare(sqlCmd);
    for(int colCount=0;colCount<dataList.length();colCount++)
    {
        QStringList rowData = dataList.at(colCount).split(",");
        for(int rowCount=0;rowCount<rowData.size();rowCount++)
        {
            sql_query.addBindValue(rowData.at(rowCount));
        }
/*
        qDebug()<<sql_query;
*/
        sql_query.exec();
    }
    sql_query.exec("commit");
    //QSqlDatabase::database().commit();
    return true;
}

bool DbOperation::insertData()
{
    sql_query.exec("PRAGMA synchronous = OFF;"); //关闭写同步执行

    //qDebug()<<"关闭写同步执行, 显式开启事务执行";
    sql_query.exec("begin"); //显式开启事务执行
    //QSqlDatabase::database().transaction();

    sql_query.prepare("INSERT INTO data VALUES(?,?)");
    for(int i=0;i<WRITECOUNT;i++)
    {
        sql_query.addBindValue(i);
        sql_query.addBindValue(WRITECOUNT-i);
        sql_query.exec();
    }
    sql_query.exec("commit");
    //QSqlDatabase::database().commit();
    return true;
}

bool DbOperation::updateData(QString sqlCmd)
{
    //修改数据
    //sql_query.exec("update student set name = \"QT\" where id = 1");
    sql_query.exec(sqlCmd);
    if(!sql_query.exec())
    {
        qDebug() << sql_query.lastError();
    }
    else
    {
        qDebug() << "updated!";
    }
    return true;
}

QList<QString> DbOperation::sqlQuery(QString sqlCmd)
{
    QList<QString> dataList;
    int ret = sql_query.exec(sqlCmd); //查询数据
    int count = 0;
    if(!ret)
    {
        qDebug()<<sql_query.lastError();
    }
    else
    {
        //qDebug()<<"sql_query: "<<sql_query.isSelect();
        while(sql_query.next()) //顺序读取每一行
        {
            QString data;
            for(int i=0;i<1000;i++)
            {
                //读取每一列,并以逗号分隔存入string
                QString colData = sql_query.value(i).toString();
                if(colData.isEmpty())
                {
                    data.chop(1); //删除最后一个","
                    break;
                }
                else
                {
                    data.append(colData);
                    data.append(",");
                }
            }
            //qDebug()<<data;
            dataList<<data;

            //qDebug()<<"-----------";
            //int heart = sql_query.value(0).toInt();
            //int breath = sql_query.value(1).toInt();
            
            //qDebug()<<QString("heart:%1    breath:%2").arg(heart).arg(breath);
            //qDebug()<<"count: "<<count<<" value:"<<sql_query.value("heart").toInt()<<" "<<sql_query.value("breath").toInt();
            count++;
        }
        //qDebug()<<"count: "<<count;
    }
    if(count <= 0)
        return dataList;

#if 0
    for(int colCount=0;colCount<dataList.length();colCount++)
    {
        QStringList rowData = dataList.at(colCount).split(",");
        //qDebug()<<"size: "<<rowData.size();
        for(int rowCount=0;rowCount<rowData.size();rowCount++)
        {
            qDebug()<<(rowData.at(rowCount))<<" ";
        }
        printf("\n");
    }
#endif
    return dataList;
}

bool DbOperation::delData(QString sqlCmd)
{
    //删除数据
    //sql_query.exec("delete from student where id = 1");
    sql_query.exec(sqlCmd);
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
    }
    else
    {
        qDebug()<<"deleted!";
    }
    return true;
}

bool DbOperation::sqlExec(QString sqlCmd)
{
    //qDebug()<<"exec sql: " <<sqlCmd;
    sql_query.exec(sqlCmd);
    if(sql_query.exec())
    {
        qDebug() << sql_query.lastError();
        return false;
    }
    else

    return true;
}
bool DbOperation::delTable(QString sqlCmd)
{
    //删除表格
    //sql_query.exec("drop table student");
    sql_query.exec(sqlCmd);
    if(sql_query.exec())
    {
        qDebug() << sql_query.lastError();
    }
    else
    {
        qDebug() << "table cleared";
    }
    return true;
}

DbOperation::~DbOperation()
{
    //关闭数据库
    db.close();
    qDebug()<<"close database";
}

