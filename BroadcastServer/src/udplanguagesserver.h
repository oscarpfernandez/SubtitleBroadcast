/******************************************************************************
 * STR2 Content Broadcast Server
 *
 * Developed by Oscar Lopes as the final project of the Multimedia Technologies
 * Master, and sponsored by CAIAC (caiac.uab.cat)
 *
 * (C)2011 [Universidad Autònoma de Barcelona]
 ******************************************************************************/

#ifndef UDPLANGUAGESSERVER_H
#define UDPLANGUAGESSERVER_H

#include <QObject>
#include <QUdpSocket>
#include <QTimer>
#include "constants.h"

class UDPLanguagesServer : public QObject
{
    Q_OBJECT
public:
    explicit UDPLanguagesServer(QObject *parent,
                                int portNumber);
    void startBroadCastingLanguages(QString languages);
    void stopBroadCastingLanguages();
    ~UDPLanguagesServer();

private:
    QUdpSocket *udpSocket;
    QString *availableLanguages;
    int port;
    QTimer *timer;
    void sendUDPMessage(QString* msg);

private slots:
    void sendLanguagesOnTick();
};

#endif // UDPLANGUAGESSERVER_H
