#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDir>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    m = NULL;
    c = NULL;
    mes = NULL;
    mouse_in_bt1=0;
    mouse_in_bt2=0;
    mouse_in_bt3=0;
    pre = 0;
    now = 0;
    ui->setupUi(this);
    ui->centralWidget->setMouseTracking(true);                  //设置鼠标监听
    this->setMouseTracking(true);
    ui->pushButton->setMouseTracking(true);
    ui->pushButton_2->setMouseTracking(true);
    ui->pushButton_3->setMouseTracking(true);

    db = new database;                                          //数据库
    db->connectdb();

    QDir::setCurrent(QApplication::applicationDirPath());          //加载css
    QByteArray bCSS;
    QFile fCSS(":/skin.qss");
    if( fCSS.open(QIODevice::ReadOnly) ){
    bCSS = fCSS.readAll();
    fCSS.close();
    }

    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(progress()));

    this->setStyleSheet(QString::fromUtf8(bCSS));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    progress(1);
    if(!(m||c||mes))
    {
        ui->label_2->hide();
        ui->label_2->deleteLater();
    }
    else if (flag!=1) {
        return;
    }
    else if(m)
    {
        return;
    }
    else if(c)
    {
        delete c;
        c = NULL;
    }
    else
    {
        delete mes;
        mes = NULL;
    }

    //QDebug()<<event.pos().x();
    m= new Monitor(this);

    m->setRoom(ui->comboBox->currentIndex());
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), m, SLOT(update(int)));
    while(true)
    {
        Sleep(1000);
        if(m)
            m->setRoom(ui->comboBox->currentIndex());
        else {
            break;
        }
    }

}

void MainWindow::on_pushButton_2_clicked()
{
    progress(2);
    if(!(m||c||mes))
    {
        ui->label_2->hide();
        ui->label_2->deleteLater();
    }
    else if (flag!=2) {
        return;
    }
    else if(m)
    {
        delete m;
        m = NULL;
    }
    else if(c)
    {
        return;
    }
    else
    {
        delete mes;
        mes = NULL;
    }
    c = new control(this);
    c->setRoom(ui->comboBox->currentIndex());
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), c, SLOT(getRoom(int)));

    flag=2;
}

void MainWindow::on_pushButton_3_clicked()
{
    progress(3);
    if(!(m||c||mes))
    {
        ui->label_2->hide();
        ui->label_2->deleteLater();
    }
    else if (flag!=3) {
        return;
    }
    else if(m)
    {
        delete m;
        m = NULL;
    }
    else if(c)
    {
        delete c;
        c = NULL;
    }
    else
    {
        return;
    }
    mes = new message(this);

    mes->setroom(ui->comboBox->currentIndex());
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), mes, SLOT(downdata(int)));
    flag=3;
}


void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
   //qDebug()<<e->pos().x();
   // qDebug()<<e->pos().y();
   e->accept();
   if(enterBtn(e->pos(),ui->pushButton)&&!mouse_in_bt1)         //进入按钮
   {
       qDebug()<<"button1";
       mouse_in_bt1=1;
       changeSize(1);
   }
   else if(!enterBtn(e->pos(),ui->pushButton)&&mouse_in_bt1){           //出按钮
       mouse_in_bt1=0;
   }
   if(enterBtn(e->pos(),ui->pushButton_2)&&!mouse_in_bt2)
   {
       qDebug()<<"button2";
       mouse_in_bt2=1;
       changeSize(2);
   }
   else if(!enterBtn(e->pos(),ui->pushButton_2)&&mouse_in_bt2){
       mouse_in_bt2=0;
   }
   if(enterBtn(e->pos(),ui->pushButton_3)&&!mouse_in_bt3)
   {
       qDebug()<<"button3";
       mouse_in_bt3=1;
       changeSize(3);
   }
   else if(!enterBtn(e->pos(),ui->pushButton_3)&&mouse_in_bt3){
       mouse_in_bt3=0;
   }
   if(!enterBtn(e->pos(),ui->pushButton)&&
           !enterBtn(e->pos(),ui->pushButton_2)&&
           !enterBtn(e->pos(),ui->pushButton_3))
   {
       if(flag==1)
           changeSize(1);
       else if (flag==2)
           changeSize(2);
       else if(flag==3)
           changeSize(3);
   }
}

bool MainWindow::enterBtn(QPoint pp, QPushButton *btn)
{
   int height = btn->height();
   int width = btn->width();
   QPoint btnMinPos = btn->pos();
   QPoint btnMaxPos = btn->pos();
   btnMaxPos.setX(btn->pos().x()+width);
   btnMaxPos.setY(btn->pos().y()+height);
   if(pp.x() >= btnMinPos.x() && pp.y() >= btnMinPos.y()
       && pp.x() <= btnMaxPos.x() && pp.y() <= btnMaxPos.y())
       return true;
   else
       return false;
}

void MainWindow::requestFinished(QNetworkReply* reply) {
    // 获取http状态码
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(statusCode.isValid())
        qDebug() << "status code=" << statusCode.toInt();

    QVariant reason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
    if(reason.isValid())
        qDebug() << "reason=" << reason.toString();

    QNetworkReply::NetworkError err = reply->error();
    if(err != QNetworkReply::NoError) {
        qDebug() << "Failed: " << reply->errorString();
    }
    else {
        // 获取返回内容
        QByteArray ba = reply->readAll();
        QJsonDocument jd = QJsonDocument::fromJson(ba);
        look = jd.object();
        //qDebug() << jo;
        qDebug()<<look["devNo"].toInt();
    }
}

void MainWindow::requestFinished_con(QNetworkReply* reply) {
    // 获取http状态码
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(statusCode.isValid())
        qDebug() << "status code=" << statusCode.toInt();

    QVariant reason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
    if(reason.isValid())
        qDebug() << "reason=" << reason.toString();

    QNetworkReply::NetworkError err = reply->error();
    if(err != QNetworkReply::NoError) {
        qDebug() << "Failed: " << reply->errorString();
    }
    else {
        // 获取返回内容
        QByteArray ba = reply->readAll();
        QJsonDocument jd = QJsonDocument::fromJson(ba);
        controller = jd.object();
        //qDebug() << jo;
        //qDebug()<<look["devNo"].toInt();
    }
}

void MainWindow::changeSize(int n)
{
    if(n==1)
    {
        ui->pushButton->setGeometry(0,0,this->geometry().height()/2.5,this->geometry().height()/2.5);
        ui->pushButton_2->setGeometry(0, this->geometry().height()/2.5,
                                      this->geometry().height()*0.3,this->geometry().height()*0.3);
        ui->pushButton_3->setGeometry(0, this->geometry().height()*0.7,
                                      this->geometry().height()*0.3, this->geometry().height()*0.3);
    }
    else if(n==2)
    {
        ui->pushButton->setGeometry(0,0,this->geometry().height()*0.3,this->geometry().height()*0.3);
        ui->pushButton_2->setGeometry(0, this->geometry().height()*0.3,
                                      this->geometry().height()*0.4,this->geometry().height()*0.4);
        ui->pushButton_3->setGeometry(0, this->geometry().height()*0.7,
                                      this->geometry().height()*0.3, this->geometry().height()*0.3);
    }
    else {
        ui->pushButton->setGeometry(0,0,this->geometry().height()*0.3,this->geometry().height()*0.3);
        ui->pushButton_2->setGeometry(0, this->geometry().height()*0.3,
                                      this->geometry().height()*0.3,this->geometry().height()*0.3);
        ui->pushButton_3->setGeometry(0, this->geometry().height()*0.6,
                                      this->geometry().height()*0.4, this->geometry().height()*0.4);
    }
}

void MainWindow::on_comboBox_activated(int index)
{

}

void MainWindow::Sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
void MainWindow::progress(int f)
{
    flag=f;
    pre = 0;
    for(now = 1;now<=100;++now)
    {
        if(pre!=now-1)
        {
            break;
        }
        Sleep(5);
        pre = now;
        ui->progressBar->setValue(now);

    }
}

