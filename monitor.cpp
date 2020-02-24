#include "monitor.h"

Monitor::Monitor(QMainWindow *u)
{

    ui=u;

    int label_x[2]={527,926};
    int label_y[3]={150,335,520};
    temperature=new QPushButton(ui);
    temperature->setText("温度:");
    temperature->setGeometry(label_x[0],label_y[0],200,100);
    QFont font("新宋体", 24, 50);
    temperature->setFont(font);
    temperature->setStyleSheet("QPushButton {"                                                                           /*pushbutton*/
                               "border: transparent;"
                               "border-radius: 6px;"
                               "background-color: transparent;"
                               "min-width: 80px;}");
    QPixmap temPic(":/new/prefix1/2-1.png");
    temP = new QLabel(ui);
    temP->setScaledContents(true);
    temP->setGeometry(label_x[0]-150,label_y[0]-50,150,120);
    temP->setPixmap(temPic);
    temP->show();
    temperature->show();

    humidity = new QPushButton(ui);
    humidity->setText("湿度:");
    humidity->setGeometry(label_x[1],label_y[0],200,100);
    humidity->setFont(font);
    humidity->setStyleSheet("QPushButton {"                                                                           /*pushbutton*/
                            "border: transparent;"
                            "border-radius: 6px;"
                            "background-color: transparent;"
                            "min-width: 80px;}");
    QPixmap humPic(":/new/prefix1/2-2.png");
    humP = new QLabel(ui);
    humP->setScaledContents(true);
    humP->setGeometry(label_x[1]-150,label_y[0]-50,150,120);
    humP->setPixmap(humPic);
    humP->show();
    humidity->show();

    waterLevel = new QPushButton(ui);
    waterLevel->setText("水位:");
    waterLevel->setGeometry(label_x[0],label_y[1],200,100);
    waterLevel ->setFont(font);
    waterLevel->setStyleSheet("QPushButton {"                                                                           /*pushbutton*/
                              "border: transparent;"
                              "border-radius: 6px;"
                              "background-color: transparent;"
                              "min-width: 80px;}");
    QPixmap watPic(":/new/prefix1/2-5.png");
    watP = new QLabel(ui);
    watP->setScaledContents(true);
    watP->setGeometry(label_x[0]-150,label_y[1]-50,150,120);
    watP->setPixmap(watPic);
    watP->show();
    waterLevel->show();

   fire = new QPushButton(ui);
   fire->setText("火灾:");
   fire->setGeometry(label_x[1],label_y[1],200,100);
   fire->setFont(font);
   fire->setStyleSheet("QPushButton {"                                                                           /*pushbutton*/
                        "border: transparent;"
                        "border-radius: 6px;"
                        "background-color: transparent;"
                        "min-width: 80px;}");
   QPixmap flaPic(":/new/prefix1/2-4.png");
   fireP = new QLabel(ui);
   fireP->setScaledContents(true);
   fireP->setGeometry(label_x[1]-150,label_y[1]-50,150,120);
   fireP->setPixmap(flaPic);
   fireP->show();
   fire->show();

   people = new QPushButton(ui);
   people->setText("有/无人");
   people->setGeometry(label_x[0],label_y[2],200,100);
   people->setFont(font);
   people->setStyleSheet("QPushButton {"                                                                           /*pushbutton*/
                      "border: transparent;"
                      "border-radius: 6px;"
                      "background-color: transparent;"
                      "min-width: 80px;}");
   QPixmap rayPic(":/new/prefix1/2-3.png");
   peoP = new QLabel(ui);
   peoP->setScaledContents(true);
   peoP->setGeometry(label_x[0]-150,label_y[2]-50,150,120);
   peoP->setPixmap(rayPic);
   peoP->show();
   people->show();

   x = new QPushButton(ui);
   x->setText("烟雾:");
   x->setGeometry(label_x[1],label_y[2],200,100);
   x->setFont(font);
   x->setStyleSheet("QPushButton {"                                                                           /*pushbutton*/
                    "border: transparent;"
                    "border-radius: 6px;"
                    "background-color: transparent;"
                    "min-width: 80px;}");
   QPixmap xPic(":/new/prefix1/2-6.png");
   xP = new QLabel(ui);
   xP->setScaledContents(true);
   xP->setGeometry(label_x[1]-150,label_y[2]-50,150,120);
   xP->setPixmap(xPic);
   xP->show();
   x->show();

}

void Monitor::setRoom(int num)
{
    roomNum=num;
    update(num);
}

void Monitor::update(int n)
{
    roomNum=n;
    QNetworkRequest request;
    QNetworkAccessManager* naManager = new QNetworkAccessManager(this);
    QMetaObject::Connection connRet = QObject::connect(naManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestFinished_down(QNetworkReply*)));
    //Q_ASSERT(connRet);

    QString s;
    s=QString::number(roomNum);                                            //获取服务器数据
    qDebug()<<"http://192.168.137.1:8080/ssm1/DHT11CurrentValue.action?DevNo="+s;
    request.setUrl(QUrl("http://192.168.137.1:8080/ssm1/GetSensorValueAdmin.action?DevNo="+s));
    QNetworkReply* reply = naManager->get(request);
}


Monitor::~Monitor()
{
    delete temperature;
    delete humidity;
    delete waterLevel;
    delete fire;
    delete people;
    delete x;
    delete temP;
    delete humP;
    delete watP;
    delete fireP;
    delete peoP;
    delete xP;
}

void Monitor::requestFinished_down(QNetworkReply* reply) {
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
        ba.replace("[","");
        ba.replace("]","");
        int mesSize=ba.split('}').size();
        QJsonDocument mesg;
        QJsonObject jo;
        QString str;
        int flag=1;
        for(int i=0;i<mesSize-1; ++i)
        {
            QByteArray s=ba.split('}')[i]+"}";
            if(s[0]==',')
                s=s.remove(0,1);
            mesg = QJsonDocument::fromJson(s);

            jo = mesg.object();
            qDebug()<<jo;
            if(roomNum == jo["devNo"].toInt())
            {
                qDebug()<<jo["devType"].toString();
                if(jo["devType"].toString()=="DHT11")
                {

                    QString tem_hum=jo["value"].toString().split(",")[1];
                    qDebug()<<"温度:"+tem_hum;
                    temperature->setText("温度:"+tem_hum);
                    tem_hum = jo["value"].toString().split(",")[0];
                    humidity->setText("湿度:"+tem_hum);
                }
//                else {
//                    temperature->setText("温度:");
//                    humidity->setText("湿度:");
//                }
                if (jo["devType"].toString()=="FLAME") {
                    if(jo["value"].toString().toInt()<50)
                    {
                        fire->setText("火灾:有");
                        fire->setStyleSheet("color: red");
                        QMediaPlayer *player = new QMediaPlayer;
                        player->setMedia(QUrl("qrc:/new/prefix1/00.mp3"));
                        player->setVolume(60);
                        player->play();

                    }
                    else {
                        fire->setText("火灾:无");
                        fire->setStyleSheet("QPushButton {"                                                                           /*pushbutton*/
                                             "border: transparent;"
                                             "border-radius: 6px;"
                                             "background-color: transparent;"
                                             "min-width: 80px;}");
                    }
                }
//                else {
//                    fire->setText("火灾:");
//                }
                if (jo["devType"].toString()=="LIQUID") {
                    if(jo["value"].toString().toDouble()<2)
                    {
                        waterLevel->setText("水位:"+jo["value"].toString());
                        waterLevel->setStyleSheet("QPushButton {"                                                                           /*pushbutton*/
                                             "border: transparent;"
                                             "border-radius: 6px;"
                                             "background-color: transparent;"
                                             "min-width: 80px;}");
                    }
                    else {
                        waterLevel->setText("水位过高");
                        waterLevel->setStyleSheet("color: red");
                        QMediaPlayer *player = new QMediaPlayer;
                        player->setMedia(QUrl("qrc:/new/prefix1/00.mp3"));
                        player->setVolume(60);
                        player->play();
                    }
                }
//                else {
//                    waterLevel->setText("水位:");
//                }
                if (jo["devType"].toString()=="IR") {
                    if(jo["value"].toString().toInt()<250)
                        people->setText("有/无人:有");
                    else {
                        people->setText("有/无人:无");
                    }
                }
//                else {
//                    people->setText("有/无人:");
//                }
                if(jo["devType"].toString()=="GAS"){
                    if(jo["value"].toString().toInt()>450)
                    {
                        x->setText("有烟雾");
                        x->setStyleSheet("color: red");
                        QMediaPlayer *player = new QMediaPlayer;
                        player->setMedia(QUrl("qrc:/new/prefix1/00.mp3"));
                        player->setVolume(60);
                        player->play();
                    }
                    else {
                        x->setText("烟雾:无");
                        x->setStyleSheet("QPushButton {"                                                                           /*pushbutton*/
                                             "border: transparent;"
                                             "border-radius: 6px;"
                                             "background-color: transparent;"
                                             "min-width: 80px;}");
                    }
                }
//                else {
//                    x->setText("烟雾");
//                }

                flag=0;
            }
        }
        if(flag)
        {
            fire->setText("火灾:");
            waterLevel->setText("水位:");
            humidity->setText("湿度:");
            temperature->setText("温度:");
            x->setText("烟雾:");
            people->setText("有/无人");
            fire->setText("火灾:");
            waterLevel->setText("水位:");
        }

    }
}

void Monitor::operateData(QJsonObject jo)
{

}
