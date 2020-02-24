#ifndef CONTROL_H
#define CONTROL_H

#include<QObject>
#include <QMainWindow>
#include<QLabel>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>
#include<QComboBox>
#include"switchcontrol.h"



class control:public QObject
{
    Q_OBJECT
private:
    QMainWindow *ui;
    SwitchControl *airConditioner;
    SwitchControl *TV;
    SwitchControl *light;
    SwitchControl *refrigerator;
    QLabel *airConditionerpic;
    QLabel *TVPic;
    QLabel *lightPic;
    QLabel *refrigeratorPic;

    bool airSta;                           //记录各开关状态
    bool TvSta;
    bool ligSta;
    bool refSta;

    int roomnum;

public:
    control(QMainWindow *u);
    ~control();
    void update();                      //更新数据
    void setRoom(int);

private slots:
    void getRoom(int);
    void upData(bool);                     //上传数据
    void requestFinished_down(QNetworkReply* reply);
};

#endif // CONTROL_H
