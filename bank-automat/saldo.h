#ifndef SALDO_H
#define SALDO_H
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QWidget>
#include "rest_api_client.h"

namespace Ui {
class Saldo;
}

class Saldo : public QWidget
{
    Q_OBJECT

public:
    explicit Saldo(QWidget *parent = nullptr);
    ~Saldo();
    void setToken(const QByteArray &newToken);
    void on_pushButton_saldo_show_clicked();
    void setCurrentAccountInUse(int accountInUse);

signals:
    void backclicked();

private slots:
    void on_pushButton_4_back_clicked();
    void getsaldoInfoSlot(QNetworkReply *reply);
    //void onStackedWidgetPageChanged(int index);

private:
    Ui::Saldo *ui;
    QNetworkAccessManager *getManager;
    QNetworkReply *reply;
    QByteArray response_data;
    QByteArray token; //saldo token
    REST_API_Client *apiClientti;
    int currentAccount;
};

#endif // SALDO_H
