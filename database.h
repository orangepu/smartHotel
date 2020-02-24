#ifndef DATABASE_H
#define DATABASE_H
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QObject>
#include<QApplication>
#include<QDebug>



class database: public QObject
{
    Q_OBJECT
public:
    explicit database(QObject *parent = 0);
    static void connectdb();

};

#endif // DATABASE_H
