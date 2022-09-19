#ifndef CONNECTIONFUN_H
#define CONNECTIONFUN_H
#include<QString>
#include<QSqlDatabase>
#include<QMessageBox>
#include<QSqlError>

bool connection(const QString &dbName)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);
    if (!db.open()) {
    QMessageBox::critical(0, QObject::tr("Database Error"),
    db.lastError().text());
    return false;
    }
    return true;
}

#endif // CONNECTIONFUN_H
