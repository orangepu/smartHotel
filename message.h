#ifndef MESSAGE_H
#define MESSAGE_H

#include <QMainWindow>
#include<QPushButton>
#include<QLabel>
#include<QTextEdit>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>
#include<QCoreApplication>

class message:public QObject
{
    Q_OBJECT
private:
    QMainWindow *ui;
    QPushButton *send;
    QTextEdit *mes;
    QTextEdit *txt;

    int room;

public:
    message(QMainWindow *ui);
    ~message();
    void setroom(int);
    void Sleep(int);

private slots:
    void updata();
    void downdata(int);
    void requestFinished_down(QNetworkReply*);


};

#endif // MESSAGE_H
