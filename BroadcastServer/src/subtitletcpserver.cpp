#include "subtitletcpserver.h"

/******************************************************************************
 * This class represents a group of connections of N clients to the server to
 * obtain subtitles for one particular language.
 ******************************************************************************/
SubtitleTCPServer::SubtitleTCPServer(QObject *parent):
        QTcpServer(parent)
{
    //the list of active connections.
    liveConnections = new QMap<int,TCPSocketHandler*>();

}

/******************************************************************************
 * Reimplements the superclass method to manage incoming connections,
 * grouping them in a list, the sending of data is performed in batch.
 * Args:
 * - int socketId: the ID of the incoming socket.
 ******************************************************************************/
void SubtitleTCPServer::incomingConnection(int socketId)
{
    TCPSocketHandler *socket = new TCPSocketHandler(this);
    socket->setSocketDescriptor(socketId);
    liveConnections->insert(socketId,socket);

}

/******************************************************************************
 * Sends text data (mainly XML) to a group of clients.
 * The sending of data is performed in batch.
 * Args:
 * - QString data: the text  to be sent.
 ******************************************************************************/
void SubtitleTCPServer::sendDataToClients(QString &data)
{

    for(int i=0; i<liveConnections->count(); i++)
    {
        TCPSocketHandler *socket = liveConnections->value(i);

        //check if the socket is still active...
        if(socket->isValid() &&
           socket->state()==QAbstractSocket::ConnectedState)
        {
            socket->sendTextData(data);
        }
        else{
            //record the index of dead connections...
            deadConnections->append(i);
        }
    }

    if(deadConnections->count() % liveConnections->count() > 25){
        // only call the claener when the number of dead
        // connections is more than 15%
        deleteInactiveSockets();
    }
}

/******************************************************************************
 * Deletes incative connections from the batch broadcast list.
 * This is performed peridically when the number of disconnected clients
 * passes a given treshold.
 ******************************************************************************/
void SubtitleTCPServer::deleteInactiveSockets()
{
    for(int i=0; i<deadConnections->count(); i++){
        int index = deadConnections->at(i);

        TCPSocketHandler *sk = liveConnections->value(index);

        delete(sk);//remove the pointer from memory.
        liveConnections->remove(index); //remove it from the map
    }

    //clear the list of ID's of "dead" sockets.
    deadConnections->clear();
}
