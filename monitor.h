#ifndef MONITOR_H
#define MONITOR_H

#include <QMainWindow>
#include<QLabel>
#include<QPushButton>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>
#include<QMediaPlayer>

class Monitor:public QObject
{
    Q_OBJECT
private:
    QMainWindow *ui;
    QPushButton *temperature;
    QPushButton *humidity;
    QPushButton *waterLevel;
    QPushButton *fire;
    QPushButton *people;
    QPushButton *x;

    QLabel *temP;
    QLabel *humP;
    QLabel *watP;
    QLabel *fireP;
    QLabel *peoP;
    QLabel *xP;

    int roomNum;
public:
    Monitor(QMainWindow *u);
    ~Monitor();
    void setRoom(int);
    void operateData(QJsonObject);

private slots:
    void update(int);                                               //更新数据
    void requestFinished_down(QNetworkReply* reply);
};

#endif // MONITOR_H
