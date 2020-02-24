#include "message.h"

message::message(QMainWindow *u)
{
    ui=u;

    send = new QPushButton(ui);
    send->setText("send");
    send->setGeometry(1000,550,120,50);
    QFont font("", 22, 50);
    send->setFont(font);
    send->show();
    connect(send, SIGNAL(clicked()), this, SLOT(updata()));

    mes = new QTextEdit(ui);
    mes->setText("");
    mes->setReadOnly(true);
    QFont mesfont("", 16, 40);
    mes->setFont(mesfont);
    mes->setGeometry(350,100,600,400);
    mes->show();


    txt = new QTextEdit(ui);
    txt->setFont(mesfont);
    txt->setGeometry(350,520,600,150);
    txt->show();

}


message::~message()
{
    delete send;
    delete mes;
    delete txt;
}

void message::setroom(int n)
{
    room=n;
    downdata(n);
}

void message::updata()
{
    QString text = txt->toPlainText();
    txt->clear();
    QString s=text;

    QNetworkRequest request;
    QNetworkAccessManager* naManager = new QNetworkAccessManager(this);
    //QMetaObject::Connection connRet = QObject::connect(naManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestFinished_down(QNetworkReply*)));
    //Q_ASSERT(connRet);
    request.setUrl(QUrl("http://192.168.137.1:8080/ssm1/MessageSendQt.action?Msg="+s+"&devNo="+QString::number(room)));
    qDebug()<<"http://192.168.137.1:8080/ssm1/MessageSendQt.action?Msg="+s+"&devNo="+QString::number(room);
    QNetworkReply* reply = naManager->get(request);
    Sleep(100);
    downdata(room);
}

void message::downdata(int n)
{
    room=n;
    QNetworkRequest request;
    QNetworkAccessManager* naManager = new QNetworkAccessManager(this);
    QMetaObject::Connection connRet = QObject::connect(naManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestFinished_down(QNetworkReply*)));
    //Q_ASSERT(connRet);
    request.setUrl(QUrl("http://192.168.137.1:8080/ssm1/MessageGetAll.action"));
    QNetworkReply* reply = naManager->get(request);
}

void message::requestFinished_down(QNetworkReply* reply)
{
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

            if(room == jo["devNo"].toString().toInt())
            {
                if(jo["Admin"].toString()=='1')
                    str+="房东:\n";
                else {
                    str+="房客:\n";
                }
                str+="时间:"+jo["time"].toString()+"\n";
                str+="留言:"+jo["Msg"].toString()+"\n";
            }
        }
        mes->setText(str);



        //qDebug() << jo.keys();
        //qDebug()<<look["devNo"].toInt();
    }
}

void message::Sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
