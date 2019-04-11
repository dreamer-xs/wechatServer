#ifndef TOKENGET_H
#define TOKENGET_H

#include <QDebug>
#include <QDateTime>
#include <QFile>



class TokenGet : public QObject
{
    Q_OBJECT

public:
    TokenGet();
    ~TokenGet();

    QString tokenValue();
    void tokenGetFromCache();

private:
    QString accessTokenValue;
    QString tokenFilePath;
    int tokenGetTime;
};

#endif // TOKENGET_H
