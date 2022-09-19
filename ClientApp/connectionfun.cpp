#include "connectionFun.h"

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
