/******************************************************************************
 * STR2 Content Broadcast Server
 *
 * Developed by Oscar Lopes as the final project of the Multimedia Technologies
 * Master, and sponsored by CAIAC (caiac.uab.cat)
 *
 * (C)2011 [Universidad Autònoma de Barcelona]
 ******************************************************************************/

#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QString>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QMessageBox>
#include <QFile>

using namespace std;

class Utils : public QObject
{
    Q_OBJECT
public:
    explicit Utils(QObject *parent = 0);
    static QString obtainMachinesIpAddress();
    static QString getCurrentUnixUserName();

};

#endif // UTILS_H
