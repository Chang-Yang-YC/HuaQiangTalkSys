#ifndef DEBUG_H
#define DEBUG_H
#include<QSqlQuery>
#include<QDebug>
#include<QSqlError>

void debug(QSqlQuery &query)
{
    int isOK = query.exec();
    if(false == isOK)
    {
        qDebug()<<"WRONG PROGRAM!";
    }
    qDebug() << query.lastError();
}

#endif // DEBUG_H
