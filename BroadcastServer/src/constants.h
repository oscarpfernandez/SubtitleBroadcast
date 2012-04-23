/******************************************************************************
 * STR2 Content Broadcast Server
 *
 * Developed by Oscar Lopes as the final project of the Multimedia Technologies
 * Master, and sponsored by CAIAC (caiac.uab.cat)
 *
 * (C)2011 [Universidad Autònoma de Barcelona]
 ******************************************************************************/

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>
#include <QListWidget>

const QString APP_NAME = "STR2 Broadcast Server";

//Languages
const QString ENGLISH = "english";
const QString SPANISH = "spanish";
const QString CATALAN = "catalan";
const QString ITALIAN = "italian";

//Port number definition
const int ENGLISH_PORT = 27500;
const int SPANISH_PORT = 27501;
const int CATALAN_PORT = 27502;
const int ITALIAN_PORT = 27503;
const int HAPTIC_PORT  = 27504;
const int LANGUAGES_PORT = 27505;


//Numer of table lines
const int NUM_OF_ROWS = 2000;
//Column in the translated text shows up...
const int INDEX_SUBS_TRANSLATED_TEXT = 5;

//Timer tick interval to broadcast available languages over UDP
const int TIMER_INTERVAL = 5000;

//Timer clock
const int GLOBAL_TIMER_INTERVAL = 314;




#endif // CONSTANTS_H
