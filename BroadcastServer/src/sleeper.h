/******************************************************************************
 * STR2 Content Broadcast Server
 *
 * Developed by Oscar Lopes as the final project of the Multimedia Technologies
 * Master, and sponsored by CAIAC (caiac.uab.cat)
 *
 * (C)2011 [Universidad Autònoma de Barcelona]
 ******************************************************************************/

#ifndef SLEEPER_H
#define SLEEPER_H

#include <QThread>

class Sleeper : public QThread
{
public:
    static void sleep(unsigned long msecs)
    {
        QThread::msleep(msecs);
    }
};

#endif // SLEEPER_H
