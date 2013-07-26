#ifndef SUBTITLES_H
#define SUBTITLES_H

#include <QTableWidget>
#include <QtGui>
#include <QString>
#include <QAction>
#include <QDomDocument>
#include "subtitlelineviewer.h"
#include "xmlprojectexport.h"

#define NUMBEROFCOLUMS 10
#define NUMBEROFROWS 2500

class Subtitles : public QTableWidget
{
    Q_OBJECT

public:
    Subtitles(QWidget *parent = 0);

    void initialize();
    void writeSoundFilesFromText(QProgressDialog *progressDialog,
                                 QString *fileType,
                                 QString *dirPath,
                                 QString *audioFilePrefixName,
                                 int columnIndex);
    bool saveSRT2Archive(QString &fullPathFileName,
                         QString &language);
    bool loadSRTArchive(QString &Nombre_Archivo);
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

signals:
    void setLanguageAndSoundFile(QString&, QString&);

};

#endif // SUBTITULOS_H


