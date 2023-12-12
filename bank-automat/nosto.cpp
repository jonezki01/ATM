#include "nosto.h"
#include "ui_nosto.h"
#include <mainwindow.h>
//#include "rest_api_client.h"
Q_GLOBAL_STATIC_WITH_ARGS(QRegularExpression, regExp, {"\\d+"});

Nosto::Nosto(QWidget *parent, int currentCardInUse, int currentAccountInUse) :
    QWidget(parent),
    ui(new Ui::Nosto)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    ui->withdrawAmountLineEdit->setMaxLength(3);
    ui->withdrawAmountLineEdit->setAlignment(Qt::AlignCenter);
    connect(ui->summa20, SIGNAL(clicked()), this, SLOT(onSummaButtonClicked()));
    connect(ui->summa40, SIGNAL(clicked()), this, SLOT(onSummaButtonClicked()));
    connect(ui->summa50, SIGNAL(clicked()), this, SLOT(onSummaButtonClicked()));
    connect(ui->summa80, SIGNAL(clicked()), this, SLOT(onSummaButtonClicked()));
    connect(ui->summa100, SIGNAL(clicked()), this, SLOT(onSummaButtonClicked()));
    connect(ui->summa150, SIGNAL(clicked()), this, SLOT(onSummaButtonClicked()));
    qDebug() << "Nosto luotu";
    currentCard = currentCardInUse;
    currentAccount = currentAccountInUse;
    ui->nostoInfoLabel->clear();
    ui->nostoSelectSumInfoNotEnougMoneyOrError->clear();
}

Nosto::~Nosto()
{
    delete ui;
    qDebug() << "Nosto tuhottu";

}

void Nosto::setToken(const QByteArray &newToken)
{
    token=newToken;
    qDebug()<<"nostossa newToken on: "<<token;
}


void Nosto::on_otherAmountButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Nosto::numPressed()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button && ui->withdrawAmountLineEdit->isVisible()) {
        ui->withdrawAmountLineEdit->insert(button->text());
    }
}

void Nosto::on_nostoTakaisin_clicked()
{
    emit nostoSignal();
}

void Nosto::on_nostoTakaisin2_clicked()
{
    ui->withdrawAmountLineEdit->clear();
    ui->nostoInfoLabel->clear();
    ui->nostoSelectSumInfoNotEnougMoneyOrError->clear();
    ui->stackedWidget->setCurrentIndex(0);
}

void Nosto::clearClicked()
{
    ui->withdrawAmountLineEdit->clear();
}

void Nosto::onSummaButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button) {
        QString sumText = button->text();
        int amount = 0;
        auto match = regExp->match(sumText);
        if (match.hasMatch()) {
            //QString matchedText = match.captured();
            amount = match.captured().toInt();
        }
        qDebug() << "amount: " << amount;
        this->withdrawAndCheckBalance(currentCard,currentAccount,amount);
    }
}

void Nosto::onInsertCardClicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

bool isDivisible(int amount) {
    //return amount % 50 == 0 || amount % 20 == 0;
    if (amount < 20 || amount > 400) { // Tarkistetaan, että summa on välillä 20-400
        return false;
    }
    if (amount == 30) { // Tarkistetaan, että summa ei ole 30
        return false;
    }
    int jaannos = amount % 10; // Lasketaan summan jakojäännös 10:llä ja tarkistetaan, että se on 0
    if (jaannos != 0) {
        return false; // Jos summa ei ole jaollinen 10:llä, sitä ei voi muodostaa seteleillä
    }
    else{
        return true;
    }
}

void Nosto::onokButtonclicked()
{
    if (ui->withdrawAmountLineEdit->isVisible()) {
        QString insertedAmount = ui->withdrawAmountLineEdit->text();
        int amount = insertedAmount.toFloat();

        if (isDivisible(amount)) {
            qDebug() << "onOkButtonClicked: " << QString::number(currentCard);
            this->withdrawAndCheckBalance(currentCard,currentAccount,amount);
        } else {
            ui->nostoInfoLabel->setText("Ei mahdollinen summa");
            ui->withdrawAmountLineEdit->clear();
        }
    }
}

void Nosto::withdrawAndCheckBalance(int cardid, int accountid, float sum)
{
    // Construct the parameters
    QUrlQuery params;
    params.addQueryItem("cardid", QString::number(cardid));
    params.addQueryItem("accountid", QString::number(accountid));
    params.addQueryItem("sum",QString::number(sum));
    QString paramsString = params.toString(QUrl::FullyEncoded);
    qDebug() << "Response:" << paramsString;
    QByteArray postData = paramsString.toUtf8();

    QString site_url = "http://127.0.0.1:3000/withdraw";
    qDebug() << "site_url: " << site_url;
    QNetworkRequest request((site_url));

    // Set header for content type
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    //WEBTOKEN ALKU
    //QByteArray token="Bearer xRstgr...";
    qDebug()<<"current token: "<<token;
    request.setRawHeader(QByteArray("Authorization"),(token));
    //WEBTOKEN LOPPU

    getManager = new QNetworkAccessManager(this);
    connect(getManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getNostoReplySlot(QNetworkReply*)));
    // Set the HTTP method and body
    getManager->post(request, postData);

}

void Nosto::getNostoReplySlot(QNetworkReply *reply)
{

    if(reply->error())
    {
        qDebug() << "ERROR:" << reply->errorString();
        SetInfoTextErrorOrNotEnougMoney("Nosto ei mahdollinen juuri nyt");
        return;
    }

    QByteArray responseBytes = reply->readAll();

    QJsonDocument jsonDocument = QJsonDocument::fromJson(responseBytes);
    if (!jsonDocument.isArray()) {
        qDebug() << "Invalid JSON response";
        ui->nostoInfoLabel->setText("Nosto ei mahdollinen juuri nyt");
        return;
    }


    // Access the first array in the JSON document
    QJsonArray outerArray = jsonDocument.array();
    if (outerArray.isEmpty()) {
        qDebug() << "Nosto ei mahdollinen juuri nyt";
        return;
    }

    // Access the first object in the first array
    QJsonArray innerArray = outerArray[0].toArray();
    QJsonObject innerObject = innerArray[0].toObject();

    // Extract 'reply' and 'amount' values
    QString sqlreply = innerObject["reply"].toString();
    double amount = innerObject["amount"].toDouble();
    QString transactionreply = innerObject["transactionreply"].toString();

    qDebug() << "Reply:" << sqlreply;
    qDebug() << "Amount:" << amount;
    qDebug() << "Transactionreply:" << transactionreply;

    if (sqlreply=="success"){
        ui->stackedWidget->setCurrentIndex(2);
        QString sum_Message = "Nostit " + QString::number(amount) + "€  Rahat tulevat hetken kuluttua";
        ui->summaLabel->setText(sum_Message);
        reply->deleteLater();
        getManager->deleteLater();
    }else{
        qDebug()<<"Tilillä ei ole katetta";
        SetInfoTextErrorOrNotEnougMoney("Tilillä ei ole katetta");
    }
}

void Nosto::SetInfoTextErrorOrNotEnougMoney(QString text)
{
    if(ui->stackedWidget->currentIndex()==1){
        ui->nostoInfoLabel->setText(text);
    }else{
    ui->nostoSelectSumInfoNotEnougMoneyOrError->setText(text);
    }
}


