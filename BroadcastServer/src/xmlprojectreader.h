/******************************************************************************
 * STR2 Content Broadcast Server
 *
 * Developed by Oscar Lopes as the final project of the Multimedia Technologies
 * Master, and sponsored by CAIAC (caiac.uab.cat)
 *
 * (C)2011 [Universidad Autònoma de Barcelona]
 ******************************************************************************/

#ifndef XMLPROJECTREADER_H
#define XMLPROJECTREADER_H

#include <QString>
#include <QFile>
#include <QXmlStreamWriter>
#include <QStringList>
#include <QMessageBox>
#include <QDateTime>
#include <QTableWidget>
#include <QFileInfo>
#include "constants.h"
#include "subtitlelinedata.h"
#include "utils.h"

const QString XML_VERSION = "1.0";

//XML tags definition...
const QString STR_EXPORT = "STR2Export";
const QString STR_PROJECT = "project";

const QString STR_PROJECTDETAILS = "project.details";
const QString STR_PROJECTNAME = "project.name";
const QString STR_PROJECTDESC = "project.description";
const QString STR_TIMESTAMP = "timestamp";
const QString STR_CREATIONTIME = "creation.time";
const QString STR_LASTMODIFTIME = "last.modification.time";
const QString STR_LANGUAGES = "languages";
const QString STR_LANG = "language";
const QString STR_AUDIO_SUB = "audio.subs";
const QString STR_AUDIO_AD = "audio.ad";
const QString STR_LANG_NAME = "name";
const QString STR_PROPERTIES = "properties";
const QString STR_XMLVERSION = "str2XMLversion";
const QString STR_SUBTITLE = "subtitle";

//Subtitle table header names.
const QString STR_INDEX = "index";
const QString STR_ENTRY_TIME = "entry.time";
const QString STR_EXIT_TIME = "exit.time";
const QString STR_PAGE_NUMBER = "page.number";
const QString STR_ACTOR_NAME = "actor.name";
const QString STR_ORIGINAL_TEXT = "original.text";
const QString STR_SUB_TRANSLATION = "subtitle.translation";
const QString STR_AUDIO_DESC = "audio.descrition";
const QString STR_COMMENT = "comment";
const QString STR_SUB_SOUND_FILE = "subtitle.sound.file";
const QString STR_AD_SOUND_FILE = "ad.sound.file";

//Subtitle table headers
const QString HEAD_ENTRY_TIME = "Entry Time";
const QString HEAD_EXIT_TIME = "Exit Time";
const QString HEAD_PAGE = "Page";
const QString HEAD_ACTOR = "Actor";
const QString HEAD_ORIG_TEXT = "Original Text";
const QString HEAD_SUB_TEXT = "Subtitle Text";
const QString HEAD_AUDIO_DESC = "Audio Description";
const QString HEAD_COMMENT = "Comment";
const QString HEAD_SUB_SOUND_FILE = "Sub Sound File";
const QString HEAD_AD_SOUND_FILE = "AD Sound File";

//Project Config Properties
const QString PROJ_CONFIG_BASE_PATH = "projbasepath";
const QString PROJ_CONFIG_NAME = "projname";
const QString PROJ_CONFIG_DESC = "projdescription";
const QString PROJ_CONFIG_LANGS = "projlangs";

class XMLProjectReader
{
public:
    XMLProjectReader(QWidget *parent);
    QMap<QString,QString> readProjectDefinitionXML(QString xmlFileName);
    bool readSubtitleXML(QMap<int,SubtitleLineData*> *subtitleDat,
                         QString &xmlFileName);

private:
    QWidget *parentWidget;
    QMap<QString,QString> readProjectXML(QFile *file);
    bool readSubtitleFromXML(QMap<int,SubtitleLineData*> *subtitleDat,
                             QFile *xmlFile);
    void showWarningDialog(QWidget *parentWidget,
                           QString message);

};

#endif // XMLPROJECTREADER_H
