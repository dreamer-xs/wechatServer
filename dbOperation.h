#ifndef __DBOPERATION_H__
#define __DBOPERATION_H__

#include <QString>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include "log.h"

//#define WRITECOUNT (10)
#define WRITECOUNT (500*60*60)
//#define WRITECOUNT (500*60*60*24)

class DbOperation
{
public:
    DbOperation(QString Name);
    ~DbOperation();
    bool createTable(QString sqlCmd);

    bool insertData(QString sqlCmd, QList<QString> dataList);

    bool updateData(QString sqlCmd);

    QList<QString> sqlQuery(QString sqlCmd);

    bool sqlExec(QString sqlCmd);
    bool delData(QString sqlCmd);
    bool delTable(QString sqlCmd);
    static int getConnection();

private:
    QSqlDatabase db;
    static int connection;
};

#endif

