/******************************************************************************
 * STR2 Content Broadcast Server
 *
 * Developed by Oscar Lopes as the final project of the Multimedia Technologies
 * Master, and sponsored by CAIAC (caiac.uab.cat)
 *
 * (C)2011 [Universidad Autònoma de Barcelona]
 ******************************************************************************/

#include "utils.h"

/******************************************************************************
 * Utility general purpose functions
 ******************************************************************************/

Utils::Utils(QObject *parent) : QObject(parent)
{
}


/******************************************************************************
 * Obtains the IP address of the current active network interface.
 * If more than one interface is running, it will obtain the IP of the first
 * one it finds.
 * On a customer machine this won't be a problem since in a typical
 * configuration, only one network card is present.
 ******************************************************************************/
QString Utils::obtainMachinesIpAddress()
{
    foreach(QNetworkInterface interface, QNetworkInterface::allInterfaces())
    {
        if (interface.flags().testFlag(QNetworkInterface::IsRunning)){

            foreach (QNetworkAddressEntry entry, interface.addressEntries())

            {
                if ( interface.hardwareAddress() != "00:00:00:00:00:00" && entry.ip().toString().contains("."))
                {
                    //items << interface.name() << entry.ip().toString();
                    return entry.ip().toString();
                }
            }
        }
    }
    return QString("");
}


/******************************************************************************
 * Obtains the current <username> in which the process is running.
 *
 * This is used to access the Apache2 user folders, from where the project must
 * be opened from, typically in /home/<username>/public_html
 ******************************************************************************/
QString Utils::getCurrentUnixUserName()
{
    qDebug("ENTRY  Utils::getCurrentUnixUserName");

    QString username(getenv("USER"));

    qDebug("\t username = "+username.toAscii());

    qDebug("EXIT  Utils::getCurrentUnixUserName");

    return username;
}
