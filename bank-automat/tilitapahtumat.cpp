#include "tilitapahtumat.h"
#include "qjsonarray.h"
#include "qjsondocument.h"
#include "qjsonobject.h"
#include "qurlquery.h"
#include "ui_tilitapahtumat.h"
#include <QtWidgets/QScrollBar>
#include <QUrlQuery>

Tilitapahtumat::Tilitapahtumat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tilitapahtumat)
{
    ui->setupUi(this);
    ui->tableTilitapahtumat->horizontalHeader()->hide();
    ui->tableTilitapahtumat->verticalHeader()->hide();
    ui->tableTilitapahtumat->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableTilitapahtumat->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    offsetti = 1;

}

Tilitapahtumat::~Tilitapahtumat()
{
    delete ui;
}

void Tilitapahtumat::setToken(const QByteArray &newToken)
{
    token=newToken;
    qDebug()<<"settoken: "<<token;
}

void Tilitapahtumat::on_pushButton_tili_backAlkuvalikko_clicked()
{
    emit tilitapahtumaBackClicked();
}

void Tilitapahtumat::clicked(int *offsetti)
{


    ui->stackedWidget->setCurrentIndex(5);

    QUrlQuery params;
    qDebug() << "Tilitapahtuma parametrit: " << QString::number(currentCard) << QString::number(currentAccount) << QString::number(*offsetti);
    params.addQueryItem("cardid", QString::number(currentCard));
    params.addQueryItem("accountid", QString::number(currentAccount));
    params.addQueryItem("offsetti", QString::number(*offsetti));

    QString paramsString = params.toString(QUrl::FullyEncoded);

    QByteArray postData = paramsString.toUtf8();

    QString site_url = "http://localhost:3000/viewtransactions";
    qDebug() << "site_url: " << site_url;

    QNetworkRequest request((site_url));


    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    //WEBTOKEN ALKU
    //QByteArray token="Bearer xRstgr...";
    qDebug()<<"current token: "<<token;
    request.setRawHeader(QByteArray("Authorization"),(token));
    //WEBTOKEN LOPPU

    getManager = new QNetworkAccessManager(this);

    connect(getManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getsaldoInfoSlot(QNetworkReply*)));

    reply = getManager->post(request, postData);

}

void Tilitapahtumat::setCurrentAccountInUse(int accountInUse)
{
    currentAccount=accountInUse;
}

void Tilitapahtumat::setCurrentCardInUse(int cardInUse)
{
    currentCard=cardInUse;
}



void Tilitapahtumat::on_pushButton_tilitapahtumat_uudemmat_clicked()
{
    if (offsetti > 1){
        offsetti = offsetti - 1;
        clicked(&offsetti);
    }

}

void Tilitapahtumat::getsaldoInfoSlot(QNetworkReply *reply)
{


    QByteArray response_data = reply->readAll();
    qDebug() << "DATA : " << response_data;

    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);

    if (!json_doc.isArray()) {
        qDebug() << "Ulompi paketti rikki";
        return;

    } else {
        QJsonArray json_array = json_doc.array().first().toArray();

        ui->tableTilitapahtumat->setRowCount(0);
        if(json_array.size()==0 ){
            offsetti -=1;

        } else {
        for (const QJsonValue &transactionValue : json_array) {
            QJsonObject transactionObject = transactionValue.toObject();

            QString action = transactionObject["action"].toString();
            int sum = transactionObject["sum"].toInt();
            QString timestamp = transactionObject["timestamp"].toString();


            int row = ui->tableTilitapahtumat->rowCount();
            ui->tableTilitapahtumat->insertRow(row);


            ui->tableTilitapahtumat->setItem(row, 0, new QTableWidgetItem(action));
            ui->tableTilitapahtumat->setItem(row, 1, new QTableWidgetItem(QString::number(sum)));
            ui->tableTilitapahtumat->setItem(row, 2, new QTableWidgetItem(timestamp));
        }
        }
    }

    reply->deleteLater();
    getManager->deleteLater();
}



void Tilitapahtumat::on_pushButton_tilitapahtumat_vanhemmat_clicked()
{
    qDebug() << "Rivien määrä: " << ui->tableTilitapahtumat->rowCount();
    if( ui->tableTilitapahtumat->rowCount() < 5)
    {
        offsetti-=1;
    }
    offsetti = offsetti + 1;
    clicked(&offsetti);
}

