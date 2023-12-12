#ifndef TILITAPAHTUMAT_H
#define TILITAPAHTUMAT_H

#include <QWidget>
#include "rest_api_client.h"




namespace Ui {
class Tilitapahtumat;
}

class Tilitapahtumat : public QWidget
{
    Q_OBJECT

public:
    explicit Tilitapahtumat(QWidget *parent = nullptr);
    ~Tilitapahtumat();
    void setToken(const QByteArray &newToken);
    void clicked(int *offsetti);
    void setCurrentAccountInUse(int accountInUse);
    void setCurrentCardInUse(int cardInUse);



    int countRows();
signals:
    void tilitapahtumaSignal();
    void tilitapahtumaBackClicked();


private slots:
    void on_pushButton_tili_backAlkuvalikko_clicked();

    void getsaldoInfoSlot(QNetworkReply *reply);
    void on_pushButton_tilitapahtumat_uudemmat_clicked();

    void on_pushButton_tilitapahtumat_vanhemmat_clicked();


private:
    Ui::Tilitapahtumat *ui;
    REST_API_Client *next;
    REST_API_Client *previous;

    REST_API_Client *apiClientti;
    QNetworkAccessManager *getManager;
    QNetworkReply *reply;
    QByteArray response_data;
    QByteArray token; //saldo token
    int currentPage = 1;
    int transactionsPerPage = 5;
    int offsetti;

    int currentCard;
    int currentAccount;

};

#endif // TILITAPAHTUMAT_H
