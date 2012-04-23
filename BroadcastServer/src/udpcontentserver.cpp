/******************************************************************************
 * STR2 Content Broadcast Server
 *
 * Developed by Oscar Lopes as the final project of the Multimedia Technologies
 * Master, and sponsored by CAIAC (caiac.uab.cat)
 *
 * (C)2011 [Universidad Autònoma de Barcelona]
 ******************************************************************************/

#include "udpcontentserver.h"

/******************************************************************************
 * The purpose of this class is to create a UDP server to broadcast single
 * lines of the Subtitle table structure to the mobile applications.
 * Also haptic signals commands are also broadcasted.
 ******************************************************************************/


UDPContentServer::UDPContentServer(QObject *parent,
                                   QMap<int,SubtitleLineData*> *subContent,
                                   int messagePort,
                                   int hapticPort) : QObject(parent)
{
    udpSocket = new QUdpSocket(this);
    msgConnPort = messagePort;
    hapticConnPort = hapticPort;
    subtitleContent = subContent;
}


/******************************************************************************
 * This function sends the XML content corresponding to a specific str2 subtitle
 * line, in a specific index.
 * Args:
 *   - int subtitleIndex: the index of the table that must be broadcasted.
 * Note: index allowed are from the range [0 ... count()-1]
 ******************************************************************************/
void UDPContentServer::sendUDPMessage(int subtitleIndex)
{
    qDebug("Entry UDPContentServer::sendUDPMessage index="+QString::number(subtitleIndex).toAscii());

    if(subtitleIndex >= subtitleContent->count() || subtitleIndex < -1)
    {
        return;
    }

    if(subtitleIndex == -1)
    {
        //Launch the empty message to clear out the mobile...
        QString emptyXmlMessage = QString("<?xml version=\"1.0\"?><subtitleline actor=\"\" subtitle=\"\" ad=\"\" subsound=\"\" adsound=\"\"/>");
        QByteArray datagram = emptyXmlMessage.toAscii();

        udpSocket->writeDatagram(datagram.data(),
                                 datagram.size(),
                                 QHostAddress::Broadcast,
                                 msgConnPort);
    }
    else
    {
        SubtitleLineData* subData = (SubtitleLineData*)subtitleContent->value(subtitleIndex+1);

        if(subData!=0){//just in case
            QString *xmlMessage = subData->xmlData;

            if(xmlMessage!=0 && xmlMessage->count()!=0)
            {
                QByteArray datagram = xmlMessage->toAscii();

                udpSocket->writeDatagram(datagram.data(),
                                         datagram.size(),
                                         QHostAddress::Broadcast,
                                         msgConnPort);
                qDebug("\tdatagram(sub) = "+datagram+
                       "\n\tport = "+QString::number(msgConnPort).toAscii());
            }
        }
    }
    qDebug("EXIT UDPContentServer::sendUDPMessage");
}

/******************************************************************************
 * Sends a haptic "shake" command to the mobile client.
 * Args:
 *  - int secondsToShake: number of seconds that the mobile shakes.
 ******************************************************************************/
void UDPContentServer::sendHapticShakeSignal(int secondsToShake)
{
    qDebug("Entry UDPContentServer::sendHapticShakeSignal index="+QString::number(secondsToShake).toAscii());

    if(secondsToShake<=0){return;}

    QByteArray datagram = "shake="+QString::number(secondsToShake).toAscii();
    udpSocket->writeDatagram(datagram.data(),
                             datagram.size(),
                             QHostAddress::Broadcast,
                             hapticConnPort);

    qDebug("\tdatagram(haptic) = "+datagram+
           "\n\tport = "+QString::number(hapticConnPort).toAscii());

    qDebug("EXIT UDPContentServer::sendHapticShakeSignal");
}

void UDPContentServer::closeSocket()
{
    udpSocket->close();
}

UDPContentServer::~UDPContentServer()
{
    delete udpSocket;
}
