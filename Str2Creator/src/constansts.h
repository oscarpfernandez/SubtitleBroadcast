#ifndef CONSTANSTS_H
#define CONSTANSTS_H

#include <QString>
#include <QFont>

const bool DEBUG_ENABLED = true;

const QString APP_NAME = "STR2 Creator";
const QString APP_VERSION = "0.9 Beta";

const QString STR2_XML_VERSION = "1.0";
const QString CONFIG_XML_FILE = "project.xml";
const QString STR2_EXPORTFILE_EXT = ".str2";

//Project Config Properties
const QString PROJ_CONFIG_BASE_PATH = "projbasepath";
const QString PROJ_CONFIG_NAME = "projname";
const QString PROJ_CONFIG_DESC = "projdescription";
const QString PROJ_CONFIG_LANGS = "projlangs";

//Languges supported
const QString ENGLISH = "english";
const QString SPANISH = "spanish";
const QString CATALAN = "catalan";
const QString ITALIAN = "italian";
const QString FRENCH = "french";

const QString AUDIO_FOLDER_NAME = "audio";

//Indexes of non-standard table widgets
const int INDEX_SUBS_START_TIME = 0;
const int INDEX_SUBS_END_TIME = 1;
const int INDEX_SUBS_ORIGINAL_TEXT = 4;
const int INDEX_SUBS_TRANSLATED_TEXT = 5;
const int INDEX_SUBS_AD = 6;
const int INDEX_SUBS_AUDIO_SUB = 8;
const int INDEX_SUBS_AUDIO_AD = 9;

//Language values
const int ENGLISH_VALUE = 0;
const int SPANISH_VALUE = 1;
const int CATALAN_VALUE = 2;
const int ITALIAN_VALUE = 3;
const int FRENCH_VALUE = 4;

//Common Font
const QFont COMMON_LABEL_FONT =  QFont("Arial",10);

//Subtitle Audio file Prefix
const QString PREFIX_SUB_AUDIO_FILE = "subtitle_";
//A.D. Audio file Prefix
const QString PREFIX_AD_AUDIO_FILE = "audiodesc_";

#endif // CONSTANSTS_H
