#include "saldo.h"
#include "ui_saldo.h"
#include <QJsonObject>


Saldo::Saldo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Saldo)
{
    ui->setupUi(this);
    ui->pushButton_saldo_show->hide();
    //ui->textEdit1->setAlignment(Qt::AlignHCenter);
}

Saldo::~Saldo()
{
    delete ui;
}

void Saldo::setToken(const QByteArray &newToken)
{
    token=newToken;
    qDebug()<<"settoken: "<<token;
}

void Saldo::on_pushButton_4_back_clicked()
{
    ui->labelShowBalanceInThisLabel->clear();
    emit backclicked();
}

void Saldo::on_pushButton_saldo_show_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
    QString site_url = "http://localhost:3000/saldo";
    QNetworkRequest request(site_url);
    QJsonObject postData;
    postData["accountid"] = currentAccount;

    QByteArray postDataBytes = QJsonDocument(postData).toJson();
    qDebug()<<"saldo buttonia painettu token on: "<<token;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", token);

    getManager = new QNetworkAccessManager(this); //getmanageri on postmanageri, en jaksa vaihtaa nimeä. Jarno

    connect(getManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getsaldoInfoSlot(QNetworkReply*)));

    reply = getManager->post(request, postDataBytes);
}

void Saldo::setCurrentAccountInUse(int accountInUse)
{
    currentAccount=accountInUse;
}

void Saldo::getsaldoInfoSlot(QNetworkReply *reply)
{

    response_data = reply->readAll();
    qDebug() << "DATA : " << response_data;

    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);

    if (json_doc.isArray()) {  // Check if the document is an array
        QJsonArray json_array = json_doc.array();
        QJsonObject json_obj = json_array.at(0).toObject();
        int balance = json_obj["balance"].toInt();
        qDebug() << "balance: " << balance;
        //ui->textEdit1->setText( QString::number(balance));
        ui->labelShowBalanceInThisLabel->setText(QString::number(balance));

    } else {
        qDebug() << "Invalid JSON format";
    }

    reply->deleteLater();
    getManager->deleteLater();
}




