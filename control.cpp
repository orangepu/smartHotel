#include "control.h"

control::control(QMainWindow *u)
{
    ui=u;

    int switch_x[2]={567,1026};
    int switch_y[3]={235,520,605};

    airConditioner = new SwitchControl(ui);
    airConditioner->setGeometry(switch_x[0],switch_y[0],66,30);
    airConditioner->show();
    QPixmap airc(":/new/prefix1/3-1.png");
    airConditionerpic = new QLabel(ui);
    airConditionerpic->setScaledContents(true);
    airConditionerpic->setGeometry(switch_x[0]-200,switch_y[0]-90,200,180);
    airConditionerpic->setPixmap(airc);
    airConditionerpic->show();
    connect(airConditioner, SIGNAL(toggled(bool)), this, SLOT(upData(bool)));


    TV= new SwitchControl(ui);
    TV->setGeometry(switch_x[0],switch_y[1],66,30);
    TV->show();
    QPixmap TVP(":/new/prefix1/3-2.png");
    TVPic = new QLabel(ui);
    TVPic->setScaledContents(true);
    TVPic->setGeometry(switch_x[0]-180,switch_y[1]-60,150,120);
    TVPic->setPixmap(TVP);
    TVPic->show();
    connect(TV, SIGNAL(toggled(bool)), this, SLOT(upData(bool)));

    light= new SwitchControl(ui);
    light->setGeometry(switch_x[1],switch_y[0],66,30);
    light->show();
    QPixmap lig(":/new/prefix1/3-3.png");
    lightPic = new QLabel(ui);
    lightPic->setScaledContents(true);
    lightPic->setGeometry(switch_x[1]-250,switch_y[0]-90,250,180);
    lightPic->setPixmap(lig);
    lightPic->show();
    connect(light, SIGNAL(toggled(bool)), this, SLOT(upData(bool)));


    refrigerator= new SwitchControl(ui);
    refrigerator->setGeometry(switch_x[1],switch_y[1],66,30);
    refrigerator->show();
    QPixmap ref(":/new/prefix1/3-4.png");
    refrigeratorPic = new QLabel(ui);
    refrigeratorPic->setScaledContents(true);
    refrigeratorPic->setGeometry(switch_x[1]-220,switch_y[1]-60,220,120);
    refrigeratorPic->setPixmap(ref);
    refrigeratorPic->show();
    connect(refrigerator, SIGNAL(toggled(bool)), this, SLOT(upData(bool)));
}

void control::upData(bool state)
{
    int flag=-1;
    if(airConditioner->isToggled()!=airSta)
    {
        flag=0;
        airSta=!airSta;
    }
    else if (TV->isToggled()!=TvSta) {
        flag=1;
        TvSta=!TvSta;
    }
    else if(light->isToggled()!=ligSta)
    {
        flag=2;
        ligSta=!ligSta;
    }
    else
    {
        flag=3;
        refSta=!refSta;
    }
    QNetworkRequest request;
    QNetworkAccessManager* naManager = new QNetworkAccessManager(this);
    //QMetaObject::Connection connRet = QObject::connect(naManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestFinished_down(QNetworkReply*)));
    //Q_ASSERT(connRet);

    QString s;
    s="devNo="+QString::number(roomnum);
    s+="&value=";
    s+=QString::number(state);
    request.setUrl(QUrl("http://192.168.137.1:8080/ssm1/SetModeAdmin.action?"+s));
    QNetworkReply* reply = naManager->get(request);
    qDebug()<<"数据上传"<<state<<flag;
}

void control::setRoom(int n)
{
    roomnum=n;
    update();
}

void control::getRoom(int num)
{
    roomnum=num;
    update();
}

void control::update()
{
    QNetworkRequest request;
    QNetworkAccessManager* naManager = new QNetworkAccessManager(this);
    QMetaObject::Connection connRet = QObject::connect(naManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestFinished_down(QNetworkReply*)));
    //Q_ASSERT(connRet);

    request.setUrl(QUrl("http://192.168.137.1:8080/ssm1/GetModeAdmin.action"));
    QNetworkReply* reply = naManager->get(request);


}

void control::requestFinished_down(QNetworkReply* reply) {
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
        for(int i=0;i<mesSize-1; ++i)
        {
            QByteArray s=ba.split('}')[i]+"}";
            if(s[0]==',')
                s=s.remove(0,1);
            mesg = QJsonDocument::fromJson(s);

            jo = mesg.object();

            if(roomnum == QString(jo["msg"].toString()).toInt())
            {
                ligSta=QString(jo["value"].toString()).toInt();
                light->setToggle(ligSta);
            }
        }

    }
}

control::~control()
{
    delete airConditioner;
    delete TV;
    delete light;
    delete refrigerator;
    delete airConditionerpic;
    delete TVPic;
    delete lightPic;
    delete refrigeratorPic;
}
