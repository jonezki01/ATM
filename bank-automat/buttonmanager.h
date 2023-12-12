// buttonmanager.h
#ifndef BUTTONMANAGER_H
#define BUTTONMANAGER_H

#include <QObject>

class ButtonManager : public QObject
{
    Q_OBJECT

public:
    ButtonManager(QObject *parent = nullptr);
    ~ButtonManager();
    void connectNumeronappaimetToSlot(QObject *targetObject, const char *slot);
    void connectWithdrawButtonsToSlots(QObject *targetObject, const char *slot);
private:
    QObject *parentObject;
};

#endif // BUTTONMANAGER_H
