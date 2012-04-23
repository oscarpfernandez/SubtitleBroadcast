#ifndef TCPSOCKETHANDLER_H
#define TCPSOCKETHANDLER_H

#include <QObject>
#include <Qt/qtcpsocket.h>

class TCPSocketHandler : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TCPSocketHandler(QObject *parent);
    void sendTextData(QString &data);

};

#endif // TCPSOCKETHANDLER_H
