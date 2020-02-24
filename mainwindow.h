#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QPushButton>
#include<QMouseEvent>
#include<QDebug>
#include<QPoint>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>
#include<QPainter>

#include"database.h"
#include"control.h"
#include"monitor.h"
#include"message.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    int flag=0;


public:
    Monitor *m;                                     //三个界面指针
    control *c;
    message *mes;
    int mouse_in_bt1;
    int mouse_in_bt2;
    int mouse_in_bt3;
    database *db;
    void changeSize(int);                           //菜单栏改变大小
    void mouseMoveEvent(QMouseEvent *e);
    bool enterBtn(QPoint pp, QPushButton *btn);
    int pre;
    int now;

    QJsonObject look;                                           //json数据
    QJsonObject controller;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void Sleep(int msec);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void requestFinished(QNetworkReply* reply);
    void requestFinished_con(QNetworkReply* reply);

    void on_comboBox_activated(int index);
    void progress(int);                                         //进度条

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
