/******************************************************************************
 * STR2 Content Broadcast Server
 *
 * Developed by Oscar Lopes as the final project of the Multimedia Technologies
 * Master, and sponsored by CAIAC (caiac.uab.cat)
 *
 * (C)2011 [Universidad Autònoma de Barcelona]
 ******************************************************************************/

#include "udplanguagesserver.h"

/******************************************************************************
 * The purpose of this class is to broadcast the available languages to the
 * mobile application so this one knows what UDP ports (mapped to a language)
 * it can listen to and receive subtitling content.
 * Without this functionality the wouldn't know in which UDP ports it should
 * listen.
 ******************************************************************************/

UDPLanguagesServer::UDPLanguagesServer(QObject *parent,
                                       int portNumber) :
QObject(parent)
{
    udpSocket = new QUdpSocket(this);
    port = portNumber;
    availableLanguages = new QString();
    timer = new QTimer;

    connect(timer,SIGNAL(timeout()),this,SLOT(sendLanguagesOnTick()));
}

/******************************************************************************
 * Starts the timer, so when it timeouts at the interval defined by
 * TIME_INTERVAL it broadcast the available languages.
 ******************************************************************************/
void UDPLanguagesServer::startBroadCastingLanguages(QString languages)
{
    availableLanguages->clear();
    availableLanguages->append(languages);

    //Turn "on" the timer that will broadcast a udp message every TIMER_INTERVAL seconds.
    timer->start(TIMER_INTERVAL);
}

/******************************************************************************
 * Starts the timer, so when it timeouts at the interval defined by
 * TIME_INTERVAL it broadcast the available languages.
 ******************************************************************************/
void UDPLanguagesServer::stopBroadCastingLanguages()
{
    //Turns "off" the timer
    timer->stop();
    udpSocket->close();
}


/******************************************************************************
 * Broadcast a UDP message.
 ******************************************************************************/
void UDPLanguagesServer::sendUDPMessage(QString* message)
{
    //qDebug("Entry UDPLanguagesServer::sendUDPMessage message = "+message->toAscii() );

    if(!message->isEmpty()){//just in case
        QByteArray datagram = message->toAscii();
        udpSocket->writeDatagram(datagram.data(),
                                 datagram.size(),
                                 QHostAddress::Broadcast,
                                 port);
        //qDebug("\tdatagram(languages) = "+datagram+
        //       "\n\tport = "+QString::number(port).toAscii());
    }
    //qDebug("EXIT UDPContentServer::sendUDPMessage");
}


/******************************************************************************
 * <SLOT>
 * Function called when the timer times out.
 ******************************************************************************/
void UDPLanguagesServer::sendLanguagesOnTick()
{
    sendUDPMessage(availableLanguages);
}

UDPLanguagesServer::~UDPLanguagesServer()
{
    delete udpSocket;
    delete availableLanguages;
    delete timer;
}
