/******************************************************************************
 * STR2 Content Broadcast Server
 *
 * Developed by Oscar Lopes as the final project of the Multimedia Technologies
 * Master, and sponsored by CAIAC (caiac.uab.cat)
 *
 * (C)2011 [Universidad Autònoma de Barcelona]
 ******************************************************************************/

#include "xmlprojectreader.h"


/******************************************************************************
 * Descritption:
 * The purpose of this class is to handle and interpret the XML content produced
 * by the content producer.
 ******************************************************************************/

XMLProjectReader::XMLProjectReader(QWidget *parent)
{
    parentWidget = parent;
}

/******************************************************************************
 * Loads the projects definitions stored in project.xml that configure the
 * contents and basic information settings.
 ******************************************************************************/
QMap<QString,QString> XMLProjectReader::readProjectDefinitionXML(QString xmlFileName)
{
    QMap<QString,QString> properties;

    if(QFile::exists(xmlFileName))
    {
        QFile *xmlFile = new QFile(xmlFileName);

        if(xmlFile->open(QIODevice::ReadOnly | QIODevice::Text))
        {
            return readProjectXML(xmlFile);
        }
        else
        {
            QString str = "The file ";
            str.append(xmlFileName).append(" could not be open...");
            showWarningDialog(parentWidget, str);
            return properties;
        }

        xmlFile->close();
        if (xmlFile->error()) {
            showWarningDialog(
                    parentWidget,
                    "It was not possible to close properly the project's' XML file!");
        }
    }
    return properties;
}

QMap<QString,QString> XMLProjectReader::readProjectXML(QFile *xmlFile)
{
    qDebug(">>Begin readProjectXML");

    //<?xml version="1.0" encoding="UTF-8"?>
    //<!--STR2 Project Basic Definition.-->
    //<project>
    //    <!--Project Name Definition.-->
    //    <project.name>The Masterpiece</project.name>
    //    <!--Small text describing the project. This can be useful for the Server operator.-->
    //    <project.description>Some description text</project.description>
    //    <!--Project creation and last modification timestamps.-->
    //    <timestamp creation.time="Fri Jul 15 18:31:25 2011" last.modification.time="Fri Jul 15 18:31:25 2011"/>
    //    <!--Definition of the languages supported by the Project-->
    //    <languages>
    //        <language audio.subs="true" audio.ad="true">Italian</language>
    //        <language audio.subs="true" audio.ad="true">Spanish</language>
    //    </languages>
    //</project>

    QStringList supportedLanguages;
    QMap<QString,QString> properties;

    QXmlStreamReader *xmlReader = new QXmlStreamReader(xmlFile);

    while(!xmlReader->atEnd() && !xmlReader->hasError()) {

        /* Read next element.*/
        xmlReader->readNext();

        /* If token is just StartDocument, we'll go to next.*/
        if(xmlReader->isStartDocument() ) {
            qDebug("\tisStartDocument...");
            continue;
        }
        /* If token is StartElement, we'll see if we can read it.*/
        if(xmlReader->isStartElement()) {
            if(xmlReader->name() == STR_PROJECT) {
                qDebug("\tReading project block...");
                continue;
            }
            else if(xmlReader->name() == STR_PROJECTDETAILS) {
                qDebug("\tReading project details block...");

                QXmlStreamAttributes attrs = xmlReader->attributes();

                const QString projName = QString(attrs.value(STR_PROJECTNAME).toString());
                const QString projDesc = QString(attrs.value(STR_PROJECTDESC).toString());
                const QString projCreationTime = QString(attrs.value(STR_CREATIONTIME).toString());
                const QString projLastModifTime = QString(attrs.value(STR_LASTMODIFTIME).toString());

                qDebug("\t- Project name: "+projName.toAscii());
                qDebug("\t- Project desc: "+projDesc.toAscii());
                qDebug("\t- Project creation time: "+projCreationTime.toAscii());
                qDebug("\t- Project modif time: "+projLastModifTime.toAscii());

                properties.insert(STR_PROJECTNAME, projName);
                properties.insert(STR_PROJECTDESC, projDesc);
                properties.insert(STR_CREATIONTIME, projCreationTime);
                properties.insert(STR_LASTMODIFTIME, projLastModifTime);

                continue;
            }
            else if(xmlReader->name() == STR_LANGUAGES){
                qDebug("\tReading Language block...");
                continue;
            }
            else if(xmlReader->name() == STR_LANG){
                qDebug("\tReading Languages block...");
                QXmlStreamAttributes attrs = xmlReader->attributes();

                const QString languageName = QString(attrs.value(STR_LANG_NAME).toString());
                const QString subtitleSupported = QString(attrs.value(STR_AUDIO_SUB).toString());
                const QString adSupported = QString(attrs.value(STR_AUDIO_AD).toString());

                qDebug("\t- Language: " + languageName.toAscii()+
                       ", Audio Sub : " + subtitleSupported.toAscii()+
                       ", Audio AD : " + adSupported.toAscii());

                //ex: english:true:true
                const QString langProp = QString(languageName);//+"@"+subtitleSupported+"@"+adSupported);

                supportedLanguages.append(langProp);

                continue;
            }
        }
        if(xmlReader->isEndElement()) {
            qDebug("\tClosing block...");
            continue;
        }
        if(xmlReader->isEndDocument()) {
            qDebug("\tEnd document block...");
            continue;
        }
    }

    /* Error handling. */
    if(xmlReader->hasError()) {
        QString errorStr = "There was a error parsing the Project's configuration file" + xmlReader->errorString();
        showWarningDialog(parentWidget, errorStr);
        xmlReader->errorString();
    }

    QString allLangs = supportedLanguages.join(":");
    properties.insert(STR_LANGUAGES,allLangs);

    xmlReader->clear();

    qDebug(">>End readProjectXML");

    return properties;
}


bool XMLProjectReader::readSubtitleXML(QMap<int,SubtitleLineData*> *subtitleDat,
                                       QString &xmlFileName)
{
    if(QFile::exists(xmlFileName))
    {
        QFile *xmlFile = new QFile(xmlFileName);

        if(xmlFile->open(QIODevice::ReadOnly | QIODevice::Text))
        {
            return readSubtitleFromXML(subtitleDat, xmlFile);
        }
        else
        {
            QString str = "The file ";
            str.append(xmlFileName).append(" could not be open...");
            showWarningDialog(parentWidget, str);
        }

        xmlFile->close();
        if (xmlFile->error()) {
            showWarningDialog(
                    parentWidget,
                    "It was not possible to close properly the Subtitle XML file!");
        }

    }
    return false;

}

bool XMLProjectReader::readSubtitleFromXML(QMap<int,SubtitleLineData*> *subtitleDat,
                                           QFile *xmlFile)
{
    /*
     Example of a language subtitle file:

    <?xml version="1.0" encoding="UTF-8"?>
    <STR2Export>
        <properties str2XMLversion="1.0" language="Spanish"/>
        <subtitle index="1"
                entry.time="00:01:57,300"
                exit.time="00:02:00,800"
                page.number="1"
                actor.name="Captain America"
                original.text="CHAPITRE UN&quot;IL ETAIT UNE FOIS...&quot;"
                subtitle.translation="Once upon a time"
                audio.descrition="Someone coughs"
                comment="Nice entry"
                subtitle.sound.file="subtitle_0.wav"
                ad.sound.file="ad_0.wav"/>
        <subtitle index="2"
                entry.time="00:02:00,800"
                exit.time="00:02:03,900"
                page.number="1"
                actor.name="Spiderman"
                original.text="&quot;DANS UNE FRANCE OCCUPEE"
                subtitle.translation="in an accupied France"
                audio.descrition="Some closes a door"
                comment="Another nice line"
                subtitle.sound.file="subtitle_1.wav"
                ad.sound.file="ad_1.wav"/>
         .....
         .....
    <STR2Export>
    */
    if(xmlFile==0){
        //just in case...
        return false;
    }

    qDebug(">>Begin readSubtitleXML: " + xmlFile->fileName().toAscii());

    QXmlStreamReader *xmlReader = new QXmlStreamReader(xmlFile);

    /*
     * Having /some/path/like/this/project/catalan/catalan.xml
     * We obtain project/catalan
     * For what? To build the sound file url!
     */
    QFileInfo fi(*xmlFile);
    QString projectPath = fi.canonicalPath();
    QStringList list = projectPath.split("/");
    QString subPath = list.at(list.count()-2);
    subPath.append("/").append(list.at(list.count()-1));

    QString ipAddress = Utils::obtainMachinesIpAddress();
    QString unixUsername = Utils::getCurrentUnixUserName();

    while(!xmlReader->atEnd() && !xmlReader->hasError()) {

        /* Read next element.*/
        xmlReader->readNext();

        /* If token is just StartDocument, we'll go to next.*/
        if(xmlReader->isStartDocument() ) {
            qDebug("\tisStartDocument...");
            continue;
        }
        /* If token is StartElement, we'll see if we can read it.*/
        if(xmlReader->isStartElement()) {
            if(xmlReader->name() == STR_EXPORT) {
                qDebug("\tReading project block...");
                continue;
            }
            else if(xmlReader->name() == STR_PROPERTIES)
            {
                qDebug("\tReading properties block");
                QXmlStreamAttributes attrs = xmlReader->attributes();
                const QString xmlVersion = QString(attrs.value(STR_XMLVERSION).toString());
                const QString language = QString(attrs.value(STR_LANG).toString());
                qDebug("\t- xmlVersion: "+xmlVersion.toAscii()+
                       "\n\t- language: "+language.toAscii());

            }
            else if(xmlReader->name() == STR_SUBTITLE) {
                qDebug("\tReading subtitle block...");

                QXmlStreamAttributes attrs = xmlReader->attributes();

                const QString index = QString(attrs.value(STR_INDEX).toString());
                const QString entryTime = QString(attrs.value(STR_ENTRY_TIME).toString());
                const QString exitTime = QString(attrs.value(STR_EXIT_TIME).toString());
                const QString pageNumber = QString(attrs.value(STR_PAGE_NUMBER).toString());
                const QString actorName = QString(attrs.value(STR_ACTOR_NAME).toString());
                const QString originalSubText = QString(attrs.value(STR_ORIGINAL_TEXT).toString());
                const QString translatedSubText = QString(attrs.value(STR_SUB_TRANSLATION).toString());
                const QString audioDesc = QString(attrs.value(STR_AUDIO_DESC).toString());
                const QString comment = QString(attrs.value(STR_COMMENT).toString());
                const QString subSoundFileName = QString(attrs.value(STR_SUB_SOUND_FILE).toString());
                const QString adSoundFileName = QString(attrs.value(STR_AD_SOUND_FILE).toString());

                //fill subtitle table fields...

                SubtitleLineData *subLineDat = new SubtitleLineData;
                subLineDat->index = new QString(index);
                subLineDat->initTime = new QString(entryTime);
                subLineDat->endTime = new QString(exitTime);
                subLineDat->pageNumber = new QString(pageNumber);
                subLineDat->actorName = new QString(actorName);
                subLineDat->originalText = new QString(originalSubText);
                subLineDat->translatedSubtitle = new QString(translatedSubText);
                subLineDat->comment = new QString(comment);
                subLineDat->audioDesc = new QString(audioDesc);
                subLineDat->subtitleSoundFile = new QString(subSoundFileName);
                subLineDat->audioDescSoundFile = new QString(adSoundFileName);
                subLineDat->xmlData = new QString;

                /*
                 * Used to build the XML struture that represents this SubtitleLineData.
                 * This is done here to improve the performance on the broadcast, so the data
                 * is ready by the time we want to launch it to the mobiles.
                 * NOTE: strings must be converted to UTF-8 otherwise they won't be correctly
                 * displayed.
                 */
                QXmlStreamWriter xmlWriterData(subLineDat->xmlData);
                xmlWriterData.writeStartDocument();
                xmlWriterData.writeStartElement("subtitleline");
                xmlWriterData.writeAttribute("actor",actorName.toUtf8());
                xmlWriterData.writeAttribute("subtitle",translatedSubText.toUtf8());
                xmlWriterData.writeAttribute("ad",audioDesc.toUtf8());
                QString subSoundFullUrl("");
                if(!subSoundFileName.isEmpty()){
                    subSoundFullUrl = QString("http://")
                                      .append(ipAddress)
                                      .append("/~").append(unixUsername).append("/")
                                      .append(subPath).append("/audio/").append(subSoundFileName);
                }
                xmlWriterData.writeAttribute("subsound",subSoundFullUrl.toUtf8());

                QString adSoundFullUrl("");
                if(!adSoundFileName.isEmpty()){
                    adSoundFullUrl = QString("http://")
                                     .append(ipAddress)
                                     .append("/~").append(unixUsername).append("/")
                                     .append(subPath).append("/audio/").append(adSoundFileName.toUtf8());
                }
                xmlWriterData.writeAttribute("adsound",adSoundFullUrl.toUtf8());
                xmlWriterData.writeEndElement();//</subtitleline>
                xmlWriterData.writeEndDocument();//end doc

                subtitleDat->insert(index.toInt(), subLineDat);

                /*qDebug("\t- index: "+index.toAscii()+
                       "\n\t- entryTime: "+entryTime.toAscii()+
                       "\n\t- exitTime: "+exitTime.toAscii()+
                       "\n\t- pageNumber: "+pageNumber.toAscii()+
                       "\n\t- actorName: "+actorName.toAscii()+
                       "\n\t- originalSubText: "+originalSubText.toAscii()+
                       "\n\t- transSubText: "+translatedSubText.toAscii()+
                       "\n\t- audioDesc: "+audioDesc.toAscii()+
                       "\n\t- comment: "+comment.toAscii()+
                       "\n\t- subSoundFile: "+subSoundFileName.toAscii()+
                       "\n\t- adSoundFileName: "+adSoundFileName.toAscii()+
                       "\n\t- xmlData: "+subLineDat->xmlData->toAscii());*/


                //go to the next xml line...
                continue;
            }
        }
        if(xmlReader->isEndElement()) {
            qDebug("\tClosing block...");
            continue;
        }
        if(xmlReader->isEndDocument()) {
            qDebug("\tEnd document block...");
            continue;
        }
    }

    /* Error handling. */
    QString errorStr("");
    if(xmlReader->hasError()) {
        errorStr = "There was a error parsing the Project's configuration file" +
                   xmlReader->errorString();
        showWarningDialog(parentWidget, errorStr);
        xmlReader->errorString();
    }

    xmlReader->clear();

    qDebug(">>End readSubtitleXML");

    return true;
}

/******************************************************************************
 * Displays a warning dialog with a custom message.
 * Args:
 *  - QWidget *parentWidget: caller widget.
 *  - QString message: custom warning message to display.
 ******************************************************************************/
void XMLProjectReader::showWarningDialog(QWidget *parentWidget,
                                         QString message)
{
    QMessageBox *warningMessage = new QMessageBox(parentWidget);
    warningMessage->setText(message);
    warningMessage->setWindowTitle("Warning");
    warningMessage->setIcon(QMessageBox::Warning);
    warningMessage->exec();

    delete warningMessage;
}



