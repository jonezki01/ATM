#include "rest_api_client.h"
#include "qjsonarray.h"
#include "qurlquery.h"
#include <QUrl>
#include <QJsonObject>
#include <QJsonDocument>
#include "mainwindow.h"

REST_API_Client::REST_API_Client(QObject *parent)
    : QObject(parent)
{
    manager = new QNetworkAccessManager(this); // Initialize the network access manager

    // Connect the finished signal to a slot for handling the response
    connect(manager, &QNetworkAccessManager::finished, this, &REST_API_Client::replyFinished);
}

void REST_API_Client::getCardData()
{
    QUrl url("http://127.0.0.1:3000/card/allcards"); // Replace with your API endpoint
    QNetworkRequest request(url);
    /*
     *QUrl url("http://127.0.0.1:3000/account/" + summa);
     * */
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    manager->get(request); // Send the request
}

void REST_API_Client::withdrawal(int summa, QString currentCardInUse)
{
    // Construct the parameters using QUrlQuery
    QUrlQuery postData;
    postData.addQueryItem("amount", QString::number(summa)); // Replace with your parameter(s)

    // Encode the parameters to a QByteArray
    QByteArray postDataByteArray = postData.toString(QUrl::FullyEncoded).toUtf8();

    // Create a QNetworkRequest and set the URL
    QNetworkRequest request(QUrl("http://127.0.0.1:3000/account/" + currentCardInUse));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    qDebug() << "http://127.0.0.1:3000/account/" + currentCardInUse;
    qDebug() << "withdrawal: "<< currentCardInUse;
    // Make the POST request with the parameters in the body
    QNetworkReply *reply = manager->post(request, postDataByteArray);
    qDebug() << "reply: "<< reply;
}

//-------Määrittää kortin / tilin joka käytössä--------
int REST_API_Client::getIdcard() const
{
    return(2);
}

void REST_API_Client::setToken(const QByteArray &newToken)
{
    token=newToken;

    qDebug()<<"newToken on: "<<token;
}




void REST_API_Client::replyFinished(QNetworkReply *reply)
{
    if(reply->error())
    {
        qDebug() << "ERROR:" << reply->errorString();
        return;
    }

    QByteArray responseBytes = reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(responseBytes);

    if (!jsonResponse.isArray()) {
        qDebug() << "Invalid JSON response";
        return;
    }

    QJsonArray jsonArray = jsonResponse.array().first().toArray(); // Access the first element of the array and ensure it's an array
    QStringList cardNames;

    for (const QJsonValue &value : jsonArray) {
        QJsonObject obj = value.toObject();
        int idcard = obj["idcard"].toInt();
        this->cardType = obj["card_type"].toString();
        cardNames.append(QString::number(idcard) + " - " + cardType); // Keeping your original line
        QString cardInfo = QString::number(obj["idcard"].toInt()) + " - " + this->cardType;
        //cardNames.append(cardInfo); // Format: "1 - Debit"
    }
    qDebug() << "Emitting cardDataReceived with data:" << cardNames;
    emit cardDataReceived(cardNames);

}

void REST_API_Client::getCardTypes(int idcard)
{
    QUrlQuery params;
    params.addQueryItem("idcard", QString::number(idcard));

    QString paramsString = params.toString(QUrl::FullyEncoded);
    qDebug() << "Response:" << paramsString;
    QByteArray postData = paramsString.toUtf8();

    QString site_url = "http://127.0.0.1:3000/selectaccount";
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
    connect(getManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(postREST_API_Client(QNetworkReply*)));
    // Set the HTTP method and body
    getManager->post(request, postData);
}

void REST_API_Client::setCurrentCard(int setThisCard)
{
    currentCard = setThisCard;
    qDebug()<< "current card: "<< currentCard;
}

void REST_API_Client::setCurrentAccount(int setThisAccount)
{ 
        currentAccount = setThisAccount;
        //currentAccount = debitAccount;
        qDebug()<<"setThisAccount: "<<setThisAccount;

}

int REST_API_Client::getCurrentAccount()
{
    return currentAccount;
}

int REST_API_Client::getCurrentCard()
{
    return currentCard;
}

int REST_API_Client::checkHowManyRows()
{
    qDebug()<<"selected data:"<<this->accountSelectionData;
    int numberOfRows = this->accountSelectionData.size();
    qDebug()<< "Data: " <<numberOfRows;
    return numberOfRows;
}

int REST_API_Client::checkIfDebitButtonIsNeeded()
{
    for (const QJsonValue &value : accountSelectionData) {
        QJsonObject obj = value.toObject();
        int idAccount = obj["idaccount"].toInt();
        QString accountType = obj["type"].toString();
        int idCustomer = obj["Customer_idCustomer"].toInt();
        int accountPriority = obj["account_priority"].toInt();

        qDebug()<<"arvo: "<<value<<idAccount<<accountType<<idCustomer<<accountPriority;
        if (accountPriority == 1 && accountType == "debit"){
            debitAccount=idAccount;
            return idAccount;
        }
    }

    return 0; //palautetaan 0 jos ei debit tiliä löydy
}

int REST_API_Client::checkIfCreditButtonIsNeeded()
{
    for (const QJsonValue &value : accountSelectionData) {
        QJsonObject obj = value.toObject();
        int idAccount = obj["idaccount"].toInt();
        QString accountType = obj["type"].toString();
        int idCustomer = obj["Customer_idCustomer"].toInt();
        int accountPriority = obj["account_priority"].toInt();

        qDebug()<<"arvo: "<<value<<idAccount<<accountType<<idCustomer<<accountPriority;
        if (accountPriority == 1 && accountType == "credit"){
            creditAccount=idAccount;
            qDebug()<<"Katsotaan mitä credit palauttaa"<<idAccount;
            return idAccount;

        }
    }

    return 0; //palautetaan 0 jos ei credit tiliä löydy
}

int REST_API_Client::checkIfSharedAccountButtonIsNeeded()
{
    int howManyAccounts = 0;
    for (const QJsonValue &value : accountSelectionData) {
        QJsonObject obj = value.toObject();
        int idAccount = obj["idaccount"].toInt();
        QString accountType = obj["type"].toString();
        int idCustomer = obj["Customer_idCustomer"].toInt();
        int accountPriority = obj["account_priority"].toInt();

        qDebug()<<"arvo: "<<value<<idAccount<<accountType<<idCustomer<<accountPriority;
        if (accountPriority == 0){
            howManyAccounts +=1;
        }
    }

    return howManyAccounts; //palautetaan 0 jos ei shared tiliä löydy, muuten palautetaan tilien määrä
}

int REST_API_Client::setSharedAccount()
{

    int howManyAccounts = 0;
    for (const QJsonValue &value : accountSelectionData) {
        QJsonObject obj = value.toObject();
        int idAccount = obj["idaccount"].toInt();
        QString accountType = obj["type"].toString();
        int idCustomer = obj["Customer_idCustomer"].toInt();
        int accountPriority = obj["account_priority"].toInt();

        qDebug()<<"arvo: "<<value<<idAccount<<accountType<<idCustomer<<accountPriority;
        if(accountPriority == 0){
            sharedAccount=idAccount;
            qDebug()<<"Katsotaan mitä shared palauttaa"<<idAccount;
            return idAccount;
        }
    }
    qDebug()<<"sharedAccount data: "<<howManyAccounts<<" jokin virhe tapahtui";
}

void REST_API_Client::postREST_API_Client(QNetworkReply *reply)
{

    if(reply->error())
    {
        qDebug() << "ERROR:" << reply->errorString();
        return;
    }

    QByteArray responseBytes = reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(responseBytes);

    if (!jsonResponse.isArray()) {
        qDebug() << "Invalid JSON response";
        return;
    }

    this->accountSelectionData = jsonResponse.array().first().toArray(); // Access the first element of the array and ensure it's an array
    qDebug()<<"Account selection data: "<<this->accountSelectionData;
    emit accountSelectionDataReady();
    getManager->deleteLater();
}

void REST_API_Client::getSharedAccountsByCardId(int idcard)
{
    QUrlQuery params;
    params.addQueryItem("idcard", QString::number(idcard));

    QString paramsString = params.toString(QUrl::FullyEncoded);
    qDebug() << "Response:" << paramsString;
    QByteArray postData = paramsString.toUtf8();

    QString site_url = "http://127.0.0.1:3000/sharedaccounts";
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
    connect(getManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(saveSharedAccountsIntoArray(QNetworkReply*)));
    // Set the HTTP method and body
    getManager->post(request, postData);
}

void REST_API_Client::saveSharedAccountsIntoArray(QNetworkReply *reply)
{

    if(reply->error())
    {
        qDebug() << "ERROR:" << reply->errorString();
        return;
    }

    QByteArray responseBytes = reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(responseBytes);

    if (!jsonResponse.isArray()) {
        qDebug() << "Invalid JSON response";
        return;
    }

    sharedAccountSelectionData = jsonResponse.array().first().toArray(); // Access the first element of the array and ensure it's an array
    qDebug()<<"Shared account selection data: "<< sharedAccountSelectionData;
    emit sharedAccountSelectionDataReady();
    getManager->deleteLater();
}
