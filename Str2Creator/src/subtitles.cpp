#include "subtitles.h"

/******************************************************************************
 * Descritption:
 * The purpose of this class is to provide a structure to represent a single
 * subtitle windows for a specific language.
 ******************************************************************************/

Subtitles::Subtitles(QWidget *parent)
    : QTableWidget (parent)
{
//    setColumnCount(NUMBEROFCOLUMS);
//    setRowCount(NUMBEROFROWS);

    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);

    action = new QAction(this);
    action->setCheckable(true);

    setFont(QFont("Courier New", 10, QFont::Normal));

    setColumnCount(0);
    setRowCount(0);

}


/******************************************************************************
 * Initializes the tables of subtitles with the defined headers and the default
 * size of the table.
 ******************************************************************************/
void Subtitles::initialize()
{
    setColumnCount(NUMBEROFCOLUMS);
    setRowCount(NUMBEROFROWS);
    setSortingEnabled(false);
    for (int i = 0 ; i < NUMBEROFROWS; i++){
        for (int j = 0 ; j < NUMBEROFCOLUMS ; j++ )
        {
/*            if(j ==INDEX_SUBS_ORIGINAL_TEXT)
            {
                QTextEdit *textEditor1 = new QTextEdit();
                textEditor1->setFont(QFont("Arial", 10));
                textEditor1->setAlignment(Qt::AlignLeft);
                setCellWidget(i,j,textEditor1);
            }
            else*/
            if(j == INDEX_SUBS_TRANSLATED_TEXT)
            {
                //this time this table widget item is handed in a different way.
                QTextEdit *textEditor2 = new QTextEdit();
                textEditor2->setFont(QFont("Arial", 12, QFont::Bold));
                textEditor2->setAlignment(Qt::AlignCenter);
                setCellWidget(i,j,textEditor2);
            }
            else{
                QTableWidgetItem *item = new QTableWidgetItem();
                item->setFont(QFont("Arial", 10));

                if(j==INDEX_SUBS_AUDIO_SUB || j==INDEX_SUBS_AUDIO_AD)
                {
                    //make the item non-editable, so the user cannot change the name of the file
                    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

                }
                setItem(i , j , item);
                setWordWrap(true);
            }
            setRowHeight(i, 90);
        }
    }
    setSortingEnabled(false);

    QStringList sl;
    sl << "Entry Time"
            << "Exit Time"
            << "Page"
            << "Actor"
            << "Original Text"
            << "Subtitle Translation & Preview"
            << "Audio Description"
            << "Comment"
            << "Subtitle Sound File"
            << "A.D. Sound File";
    setHorizontalHeaderLabels(sl);

    /*
     * Note: update this list everytime the table header is changed.
     * this one holds the table header name's to be used in the XML export
     * of this subtitle table.
     */
    QStringList list;
    list << STR_ENTRY_TIME
            << STR_EXIT_TIME
            << STR_PAGE_NUMBER
            << STR_ACTOR_NAME
            << STR_ORIGINAL_TEXT
            << STR_SUB_TRANSLATION
            << STR_AUDIO_DESC
            << STR_COMMENT
            << STR_SUB_SOUND_FILE
            << STR_AD_SOUND_FILE;
    tableHeadersXMLNames = new QStringList(list);

    setColumnWidth(0,90);
    setColumnWidth(1,90);
    setColumnWidth(2,60);
    setColumnWidth(3,90);
    setColumnWidth(4,400);
    setColumnWidth(5,400);
    setColumnWidth(6,250);
    setColumnWidth(7,250);
    setColumnWidth(8,150);
    setColumnWidth(9,150);
    size().setWidth(700);

}

/******************************************************************************
 * Exports the contents of a given column to the Text-to-Speech module,
 * generating to some folder, those files sequentially, with a common prefix
 * name plus the id of that specific line.
 * Args:
 * - columnIndex: the column index we want to generate the audio
 * - fileName: the name to be given to the file
 * - filePath: the folder path to where the file is going to be generated
 * - voiceName_ the TTS voice name to be used
 * - audioFileType: the audio file type to be generated (tipically a riff)-
 *
 ******************************************************************************/
void Subtitles::writeSoundFilesFromText(QProgressDialog *progressDialog,
                                        QString *fileType,
                                        QString *dirPath,
                                        QString *audioFilePrefixName,
                                        int columnIndex)
{
    QDir *dir = new QDir();
    if(!dir->exists(*dirPath)){
        //create audio base directory
        dir->mkdir(*dirPath);
    }

    QString *waveFileName = new QString;
    QString *oggFileName = new QString;
    QString *fullPath = new QString;
    QString *subText = new QString;

    progressDialog->show();

    for(int j=0; j < rowCount(); j++){
        subText->clear();

        if(columnIndex == INDEX_SUBS_TRANSLATED_TEXT)
        {
            QTextEdit *textEdit = (QTextEdit*)cellWidget(j,columnIndex);
            subText->append(textEdit->toPlainText());
        }
        else{
            subText->append(item(j, columnIndex)->text());
        }

        if(subText->count()!=0 && !subText->isNull()){
            waveFileName->clear();
            oggFileName->clear();
            fullPath->clear();

            waveFileName->append("tempFile");
            waveFileName->append(QString::number(j));
            waveFileName->append(QString(".wav"));

            oggFileName->append(audioFilePrefixName);
            oggFileName->append(QString::number(j));
            oggFileName->append(QString(".ogg"));

            fullPath = new QString(*dirPath);
            fullPath->append(waveFileName);

            /*
             * The generated file is a .wav type, very large in size, not good for streaming...
             * Let's convert it to a .ogg format (no patents and lot less size).
             * We're using vorbis-tools debian package.
             */
            const QString oggEncApp = "oggenc";
            const QString waveFile(*fullPath);
            const QString oggFile(*oggFileName);
            const QString workingDir(*dirPath);
            Utils::createOggFile(waveFile,oggFile,workingDir);

            //Now delete the wave file...
            const QString fileToDelete(*fullPath);
            QFile::remove(fileToDelete);


            if(columnIndex==5)//subtitle
            {
                item(j,8)->setText(*oggFileName);
                item(j,8)->setBackgroundColor(Qt::yellow);
                item(j,8)->setIcon(QIcon(":/icons/audio.png"));
                item(j,8)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            }
            else if(columnIndex==6)//ADescription
            {
                item(j,9)->setText(*oggFileName);
                item(j,9)->setBackgroundColor(Qt::yellow);
                item(j,9)->setIcon(QIcon(":/icons/audio.png"));
                item(j,9)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            }



        }
        progressDialog->setValue(progressDialog->value()+1);
    }

    delete(dir);
    delete(subText);
    delete(waveFileName);
    delete(oggFileName);
    delete(fullPath);
}

QStringList* Subtitles::getTableHeaderXMLNames()
{
    return tableHeadersXMLNames;
}

//bool QTableWidget::keyPressEvent(QKeyEvent* event){
//    if(event->key() == Qt::Key_Enter){
//        //ignore
//    }
//    else{
//        Subtitles::keyPressEvent(event);
//    }
//}


bool Subtitles::saveSRT2Archive(QString &languageDir, QString &language)
{
    XMLProjectExport *xmlHandler = new XMLProjectExport(this);
    xmlHandler->writeSubtitleXML(this, languageDir, language);

    delete xmlHandler;

    return true;
}

// Función Abrir_Archivo. Abre un archivo srt o srt2.
bool Subtitles::loadSRTArchive(QString &fileFullPath)
{
    /********************************
     * Example of a SRT file...
    1
    00:03:09,820 --> 00:03:14,332
    Hola. Me Llamo Forrest.
    Forrest Gump.

    2
    00:03:19,771 --> 00:03:21,659
    ¿ Quiere un chocolate?

    3
    00:03:24,315 --> 00:03:28,123
    Me podría comer un millón
    y medio de chocolates.

    4
    00:03:28,667 --> 00:03:34,043
    Mi mamá siempre decía: "La vida
    es como una caja de chocolates."
    **********************************/

    QFile *file = new QFile(fileFullPath);
    QTextStream *inTextStream = new QTextStream(file);
    inTextStream->setCodec("UTF-8");

    int index=0;//index of the SRT
    QString line;//current read line
    QString text;//subtitle text obtained from several reads.
    QString startTime;
    QString endTime;

    bool isAD = false;

    if(!file->open(QFile::ReadOnly))
    {
        Utils::showWarningDialog(this, "Cannot open file. Check permissions!");
        return false;
    }

    while(!inTextStream->atEnd()){
        //read line from the stream
        line = inTextStream->readLine();

        //check if the line is a number
        bool okIsNumber;
        int valueIndex = line.toInt(&okIsNumber);

        if(line.isEmpty() && index>0) {

            if(isAD){
                item(index-1, INDEX_SUBS_AD)->setText(text);
                isAD = false;
            }
            else{
                //write text on the table on row==index-1
                QTextEdit* textEdit = (QTextEdit*)cellWidget(index-1, INDEX_SUBS_TRANSLATED_TEXT);
                textEdit->setText(text);
            }

            //item(index-1,INDEX_SUBS_ORIGINAL_TEXT)->setText(text);
            item(index-1,INDEX_SUBS_START_TIME)->setText(startTime.trimmed());
            item(index-1,INDEX_SUBS_END_TIME)->setText(endTime.trimmed());
            //set start and stop time.
            //clear the text to read a new subtitle line.
            text.clear();

            //no more verifications to do. skip to the next line...
            continue;
        }
        else if(okIsNumber){
            //is a number so clear the accumulated text buffer.
            text.clear();
            index = valueIndex;
        }
        else if(line.contains("-->"))
        {
            QStringList list = line.split("-->");
            startTime = list.first();
            endTime = list.last();
        }
        else{
            if(line.contains("<AD>")){
                isAD = true;
                line = line.replace("<AD>", "");
            }

            //is simple text
            text.append(line).append("\n");
        }
    }

    file->close();

    delete(inTextStream);
    delete(file);
}

QAction* Subtitles::getEditorAction()
{
    return action;
}
