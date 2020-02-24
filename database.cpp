#include "database.h"

database::database(QObject *parent) :
    QObject(parent)
{

}

void database::connectdb()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    QString strDbName = QApplication::applicationDirPath() + "/set.db";
    db.setDatabaseName(strDbName);
//    if (!db.open()) {
//        qDebug()<<("Cannot open database");
//    }
//    else {
//        qDebug()<<("open database");
//    }
}
