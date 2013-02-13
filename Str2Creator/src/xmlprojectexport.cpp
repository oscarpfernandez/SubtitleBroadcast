#include "xmlprojectexport.h"
#include <stdio.h>
#include <subtitles.h>

/******************************************************************************
 * Descritption:
 * The purpose of this class is to provide import/export functionalities
 * for the Subtitles XML and also the projects basic configurations, in easy
 * api.
 ******************************************************************************/

XMLProjectExport::XMLProjectExport(QWidget *parent)
{
    parentWidget = parent;
}


QMap<QString,QString> XMLProjectExport::readProjectDefinitionXML(QString xmlFileName)
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
            Utils::showWarningDialog(parentWidget, str);
            return properties;
        }
    }
    return properties;
}

QMap<QString,QString> XMLProjectExport::readProjectXML(QFile *xmlFile)
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
    //        <language audio.subs="false" audio.ad="false">Spanish</language>
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
                       ", Audio Sub ?: " + subtitleSupported.toAscii()+
                       ", Audio AD ?: " + adSupported.toAscii());

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
        Utils::showWarningDialog(parentWidget, errorStr);
        xmlReader->errorString();
    }

    QString allLangs = supportedLanguages.join(":");
    properties.insert(STR_LANGUAGES,allLangs);

    xmlReader->clear();

    qDebug(">>End readProjectXML");

    return properties;
}

/******************************************************************************
 * Writes the project definition XML, containing the base configurations
 * of the subtitle project.
 * Args:
 *  - QString projectName: the project's name
 *  - QString projectDescription: projects description
 *  - QString projectBasePath: project's base directory path
 *  - QString xmlFileName: name of the file to be generated.
 *  - QStringList supportedLangs: list of project's supported languages.
 * Returns:
 *  - true if writing the file was successfull, false otherwise.
 ******************************************************************************/
bool XMLProjectExport::writeProjectDefinitionXml(QString projectName,
                                                 QString projectDescription,
                                                 QString projectBasePath,
                                                 QString xmlFileName,
                                                 QStringList supportedLangs)
{
    QString fullFilePath;
    fullFilePath.append(projectBasePath);
    fullFilePath.append("/");
    fullFilePath.append(xmlFileName);

    if (QFile::exists(fullFilePath))
    {
        QMessageBox *msgBox = new QMessageBox(parentWidget);
        msgBox->setText("This folder already contains a STR2 project.\nDo you wish to overwrite it ?");
        msgBox->setStandardButtons(QMessageBox::Ok| QMessageBox::Cancel);
        int answer = msgBox->exec();

        switch (answer) {
        case QMessageBox::Ok:
            {
                return writeProjectXML(fullFilePath,
                                       projectName,
                                       projectDescription,
                                       supportedLangs);
            }
        case QMessageBox::Cancel:
            {
                return false;
            }
        }
    }


    //if the file does not exit just write it...
    return writeProjectXML(fullFilePath,
                           projectName,
                           projectDescription,
                           supportedLangs);
}

/******************************************************************************
 * Writes the project definition XML, containing the base configurations
 * of the subtitle project.
 * Args:
 *  - QString fullFilePath: xml file full path.
 *  - QString projectName: project's name.
 *  - QString projectDescription: project's description
 *  - QStringList supportedLangs: supported languages.
 * Returns:
 *  - true if writing the file was successfull, false otherwise.
 ******************************************************************************/
bool XMLProjectExport::writeProjectXML(QString fullFilePath,
                                       QString projectName,
                                       QString projectDescription,
                                       QStringList supportedLangs)
{
    QFile *file = new QFile(fullFilePath);

    if (!file->open(QIODevice::WriteOnly | QIODevice::Text)) {
        Utils::showWarningDialog(parentWidget, "Cannot write to file... Please check folder's permissions!");
        return false;
    }


    QXmlStreamWriter *xmlWriter = new QXmlStreamWriter();
    xmlWriter->setDevice(file);

    xmlWriter->setAutoFormatting(true);
    xmlWriter->writeStartDocument();

    xmlWriter->writeStartElement(STR_PROJECT);

    xmlWriter->writeStartElement(STR_PROJECTDETAILS);
    xmlWriter->writeAttribute(STR_PROJECTNAME,projectName);
    xmlWriter->writeAttribute(STR_PROJECTDESC,projectDescription);
    xmlWriter->writeAttribute(STR_CREATIONTIME, QDateTime::currentDateTime().toString());
    xmlWriter->writeAttribute(STR_LASTMODIFTIME, QDateTime::currentDateTime().toString());
    xmlWriter->writeEndElement();//project details


    xmlWriter->writeStartElement(STR_LANGUAGES);
    for(int i=0; i<supportedLangs.count();i++)
    {
        xmlWriter->writeStartElement(STR_LANG);
        xmlWriter->writeAttribute(STR_LANG_NAME,supportedLangs.at(i));
        xmlWriter->writeAttribute(STR_AUDIO_SUB,"false");
        xmlWriter->writeAttribute(STR_AUDIO_AD,"false");
        xmlWriter->writeEndElement();
    }
    xmlWriter->writeEndElement();//languages

    xmlWriter->writeEndElement();//project

    xmlWriter->writeEndDocument();


    file->close();
    if (file->error()) {
        Utils::showWarningDialog(parentWidget, "It was not possible to close properly the project's' XML file!");
        return false;
    }

    delete xmlWriter;//free mem
    return true;
}

/******************************************************************************
 * Writes the project subtitles XMLs, containing the contents
 * of each subtitles language.
 * Args:
 *  - QTableWidget *subtitle: the subtitles table.
 *  - QString &languageDir: the languages's directory (contained under the
 *    project's base path).
 *  - QString &language: the supported language that is going to be saved.
 * Returns:
 *  - true if writing the file was successfull, false otherwise.
 ******************************************************************************/
bool XMLProjectExport::writeSubtitleXML(QTableWidget *subtitle,
                                        QString &languageDir,
                                        QString &language)
{
    QString *filePath = new QString(languageDir);
    filePath->append("/");
    filePath->append(language);
    filePath->append(".xml");

    QFile *file = new QFile(*filePath);

    if (!file->open(QFile::WriteOnly)) {
        Utils::showWarningDialog(parentWidget, "Cannot write to file... Please check folder's permissions!");
        return false;
    }

    QXmlStreamWriter *xmlWriter = new QXmlStreamWriter(file);

    xmlWriter->setAutoFormatting(true);
    xmlWriter->writeStartDocument();

    xmlWriter->writeStartElement(STR_EXPORT);

    xmlWriter->writeStartElement(STR_PROPERTIES);
    xmlWriter->writeAttribute(STR_XMLVERSION, XML_VERSION);
    xmlWriter->writeAttribute(STR_LANG, language);
    xmlWriter->writeEndElement();

    QStringList *tableHeaders = ((Subtitles*)subtitle)->getTableHeaderXMLNames();


    for(int i=0; i < subtitle->rowCount(); i++)
    {
        xmlWriter->writeStartElement(STR_SUBTITLE);

        xmlWriter->writeAttribute(STR_INDEX , QString::number(i+1));

        for(int k=0; k<tableHeaders->count(); k++)
        {
            QString str;
            /*if(k == INDEX_SUBS_ORIGINAL_TEXT)
            {
                QTextEdit *textEdit = (QTextEdit*)subtitle->cellWidget(i,k);
                str = textEdit->toPlainText();
            }
            else*/ if(k == INDEX_SUBS_TRANSLATED_TEXT)
            {
                       QTextEdit *textEdit = (QTextEdit*)subtitle->cellWidget(i,k);
                       str = textEdit->toPlainText();
                   }
                   else{
                       str = subtitle->item(i,k)->text();
                   }
                   xmlWriter->writeAttribute( tableHeaders->at(k) , str );
               }

        xmlWriter->writeEndElement(); //subtitle
    }

    xmlWriter->writeEndDocument();

    file->flush();
    file->close();
    if (file->error()) {
        Utils::showWarningDialog(
                parentWidget,
                "It was not possible to close properly the Subtitle XML file!");
        return false;
    }

    delete file;

    return true;
}

bool XMLProjectExport::readSubtitleXML(QTableWidget *subtitle, QString &xmlFileName)
{
    if(QFile::exists(xmlFileName))
    {
        QFile *xmlFile = new QFile(xmlFileName);

        if(xmlFile->open(QIODevice::ReadOnly | QIODevice::Text))
        {
            return readSubtitleXML(subtitle, xmlFile);
        }
        else
        {
            QString str = "The file ";
            str.append(xmlFileName).append(" could not be open...");
            Utils::showWarningDialog(parentWidget, str);
        }
    }
    return false;

}

bool XMLProjectExport::readSubtitleXML(QTableWidget *subtitle,
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

//                qDebug("Number of attrs found: " + QString::number(attrs.count()).toAscii());

//                for(int i=0; i<attrs.count(); i++)
//                {
//                    QXmlStreamAttribute a = (QXmlStreamAttribute)attrs.at(i);
//                    qDebug("atributo: " + a.name().toString().toAscii() + " value="+a.value().toString().toAscii());
//                }

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

                QList<QString> infoList;
                infoList << entryTime << exitTime
                        << pageNumber << actorName
                        << originalSubText << translatedSubText
                        << audioDesc << comment
                        << subSoundFileName << adSoundFileName;

                qDebug() <<"\t- index: " << index
                       <<"\n\t- entryTime: " << entryTime
                       <<"\n\t- exitTime: "<< exitTime
                       <<"\n\t- pageNumber: " << pageNumber
                       <<"\n\t- actorName: " << actorName
                       <<"\n\t- originalSubText: " << originalSubText
                       <<"\n\t- transSubText: " << translatedSubText
                       <<"\n\t- audioDesc: " << audioDesc
                       <<"\n\t- comment: " << comment
                       <<"\n\t- subSoundFile: " << subSoundFileName
                       <<"\n\t- adSoundFileName: "<< adSoundFileName;

                for(int column=0; column<infoList.count(); column++){
                    if(column ==INDEX_SUBS_TRANSLATED_TEXT){
                        QTextEdit* textEdit = (QTextEdit*)subtitle->cellWidget(index.toInt()-1, column);
                        textEdit->setText(translatedSubText);
                    }
                    else if(column == INDEX_SUBS_AUDIO_SUB || column == INDEX_SUBS_AUDIO_AD){
                           QTableWidgetItem* item = subtitle->item(index.toInt()-1, column );
                           item->setText(infoList.at(column));
                           if(!infoList.at(column).isEmpty()){
                               item->setIcon(QIcon(":/icons/audio.png"));
                           }

                    }

                    else{
                        subtitle->item(index.toInt()-1, column )->setText(infoList.at(column));
                    }
                }


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
        Utils::showWarningDialog(parentWidget, errorStr);
        xmlReader->errorString();
    }

    xmlReader->clear();

    qDebug(">>End readSubtitleXML");

    return true;
}




