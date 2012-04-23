/******************************************************************************
 * STR2 Content Broadcast Server
 *
 * Developed by Oscar Lopes as the final project of the Multimedia Technologies
 * Master, and sponsored by CAIAC (caiac.uab.cat)
 *
 * (C)2011 [Universidad Autònoma de Barcelona]
 ******************************************************************************/

#ifndef UDPCONTENTSERVER_H
#define UDPCONTENTSERVER_H

#include <QObject>
#include <QMap>
#include <QUdpSocket>
#include "subtitlelinedata.h"

class UDPContentServer : public QObject
{
    Q_OBJECT
public:
    explicit UDPContentServer(QObject *parent,
                              QMap<int,SubtitleLineData*> *subContent,
                              int messagePort,
                              int hapticPort);
    void sendUDPMessage(int subtitleIndex);
    void sendHapticShakeSignal(int secondsToShake);
    void closeSocket();
    ~UDPContentServer();

private:
    QMap<int,SubtitleLineData*> *subtitleContent;
    QUdpSocket *udpSocket;
    int msgConnPort;
    int hapticConnPort;

};

#endif // UDPCONTENTSERVER_H
