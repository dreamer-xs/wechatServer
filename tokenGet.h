#ifndef TOKENGET_H
#define TOKENGET_H

#include <QDebug>
#include <QDateTime>
#include <QFile>
#include "dbOperation.h"



class TokenGet : public QObject
{
    Q_OBJECT

public:
    TokenGet();
    ~TokenGet();

    QString tokenValue();
    void tokenGetFromDB();

private:
    QString accessTokenValue;
    QString tokenFilePath;
    int tokenGetTime;
    DbOperation* db;
};

#endif // TOKENGET_H
