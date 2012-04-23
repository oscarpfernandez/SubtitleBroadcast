#ifndef XMLPROJECTEXPORT_H
#define XMLPROJECTEXPORT_H

#include <QString>
#include <QFile>
#include <QXmlStreamWriter>
#include <QStringList>
#include <QMessageBox>
#include <QDateTime>
#include <QTableWidget>
#include "src\include\utils.h"

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

class XMLProjectExport
{
public:
    XMLProjectExport(QWidget *parent);
    QMap<QString,QString> readProjectDefinitionXML(QString xmlFileName);
    bool writeProjectDefinitionXml(QString projectName,
                                   QString projectDescription,
                                   QString projactBasePath,
                                   QString xmlFileName,
                                   QStringList supportedLangs);
    bool writeSubtitleXML(QTableWidget *subtitle,
                          QString &languageDir,
                          QString &language);
    bool readSubtitleXML(QTableWidget *subtitle,
                         QString &xmlFileName);

private:
    QWidget *parentWidget;
    bool writeProjectXML(QString fullFilePath,
                         QString projectName,
                         QString projectDescription,
                         QStringList supportedLangs);
    QMap<QString,QString> readProjectXML(QFile *file);
    bool readSubtitleXML(QTableWidget *subtitle,
                         QFile *xmlFile);

};

#endif // XMLPROJECTEXPORT_H
