#ifndef SUBTITLETCPSERVER_H
#define SUBTITLETCPSERVER_H

#include <QObject>
#include <Qt/qtcpserver.h>
#include <QMap>
#include <QList>

#include "tcpsockethandler.h"

class SubtitleTCPServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit SubtitleTCPServer(QObject *parent);

private:
    QMap<int,TCPSocketHandler*> *liveConnections;
    QList<int> *deadConnections;
    void incomingConnection(int socketId);
    void sendDataToClients(QString &data);
    void deleteInactiveSockets();

};

#endif // SUBTITLETCPSERVER_H

