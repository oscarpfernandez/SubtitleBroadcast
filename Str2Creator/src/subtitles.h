#ifndef SUBTITLES_H
#define SUBTITLES_H

#include <QTableWidget>
#include <QtGui>
#include <QString>
#include <QAction>
#include <QDomDocument>
#include "subtitlelineviewer.h"
#include "xmlprojectexport.h"
#include "festivalaudiogenerator.h"



#define NUMBEROFCOLUMS 10
#define NUMBEROFROWS 2000

class Subtitles : public QTableWidget
{
    Q_OBJECT

public:
    Subtitles(QWidget *parent = 0);

    void initialize();
    void writeSoundFilesFromText(FestivalAudioGenerator *festAudioGen,
                                 QProgressDialog *progressDialog,
                                 QString *fileType,
                                 QString *dirPath,
                                 QString *audioFilePrefixName,
                                 int columnIndex);
    bool saveSRT2Archive(QString &fullPathFileName,
                         QString &language);
    bool loadSRTArchive(QString &Nombre_Archivo);
    void eraseLanguage(QString &Idioma_);
    QStringList* getTableHeaderXMLNames();
    QAction* getEditorAction();

private:
    SubtitleLineViewer *subsLineViewer;
    QAction *action;
    QStringList *tableHeadersXMLNames;
    bool writeSubtitleXML(QFile *file,
                          QString &language);
    bool writeSubtitleXMLDOM(QFile *file,
                             QString &language);

public slots:
    void loadAudiodescriptor(int indice_,
                             QString nombre_,
                             QString delay_);
    void loadAudioSubtitle(int indice_,
                           QString nombre_);
    void addSRTArchive (QString Nombre_,
                        QString Idioma_);

signals:
    void setLanguageAndSoundFile(QString&, QString&);

};

#endif // SUBTITULOS_H


