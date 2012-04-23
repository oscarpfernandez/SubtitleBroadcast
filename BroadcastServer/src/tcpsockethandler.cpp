#include "tcpsockethandler.h"

/******************************************************************************
 * This classes represents ONE socket connection to one client, managing the
 * reading and writing to a particular client.
 ******************************************************************************/

TCPSocketHandler::TCPSocketHandler(QObject *parent) :
        QTcpSocket(parent)
{

}

/******************************************************************************
 * Sends text data to one client.
 * Args:
 * - QString &data: the text to send.
 ******************************************************************************/
void TCPSocketHandler::sendTextData(QString &data)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_3);
    out << data;

    write(block);
}

