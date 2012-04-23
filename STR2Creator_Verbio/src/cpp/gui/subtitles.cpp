#include "src\include\subtitles.h"

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
            setRowHeight(i, 50);
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
//void Subtitles::writeSoundFilesFromText(FestivalAudioGenerator *festAudioGen,
//                                        QProgressDialog *progressDialog,
//                                        QString *fileType,
//                                        QString *dirPath,
//                                        QString *audioFilePrefixName,
//                                        int columnIndex)
//{
//    QDir *dir = new QDir();
//    if(!dir->exists(*dirPath)){
//        //create audio base directory
//        dir->mkdir(*dirPath);
//    }

//    QString *waveFileName = new QString;
//    QString *oggFileName = new QString;
//    QString *fullPath = new QString;
//    QString *subText = new QString;

//    progressDialog->show();

//    for(int j=0; j < rowCount(); j++){
//        subText->clear();

//        if(columnIndex == INDEX_SUBS_TRANSLATED_TEXT)
//        {
//            QTextEdit *textEdit = (QTextEdit*)cellWidget(j,columnIndex);
//            subText->append(textEdit->toPlainText());
//        }
//        else{
//            subText->append(item(j, columnIndex)->text());
//        }

//        if(subText->count()!=0 && !subText->isNull()){
//            waveFileName->clear();
//            oggFileName->clear();
//            fullPath->clear();

//            waveFileName->append("tempFile");
//            waveFileName->append(QString::number(j));
//            waveFileName->append(QString(".wav"));

//            oggFileName->append(audioFilePrefixName);
//            oggFileName->append(QString::number(j));
//            oggFileName->append(QString(".ogg"));

//            fullPath = new QString(*dirPath);
//            fullPath->append(waveFileName);

//            festAudioGen->generateAudioFile(subText,fullPath,fileType);

//            /*
//             * The generated file is a .wav type, very large in size, not good for streaming...
//             * Let's convert it to a .ogg format (no patents and lot less size).
//             * We're using vorbis-tools debian package.
//             */
//            const QString oggEncApp = "oggenc";
//            const QString waveFile(*fullPath);
//            const QString oggFile(*oggFileName);
//            const QString workingDir(*dirPath);
//            Utils::createOggFile(waveFile,oggFile,workingDir);

//            //Now delete the wave file...
//            const QString fileToDelete(*fullPath);
//            QFile::remove(fileToDelete);


//            if(columnIndex==5)//subtitle
//            {
//                item(j,8)->setText(*oggFileName);
//                item(j,8)->setBackgroundColor(Qt::yellow);
//                item(j,8)->setIcon(QIcon(":/icons/audio.png"));
//                item(j,8)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
//            }
//            else if(columnIndex==6)//ADescription
//            {
//                item(j,9)->setText(*oggFileName);
//                item(j,9)->setBackgroundColor(Qt::yellow);
//                item(j,9)->setIcon(QIcon(":/icons/audio.png"));
//                item(j,9)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
//            }



//        }
//        progressDialog->setValue(progressDialog->value()+1);
//    }

//    delete(dir);
//    delete(subText);
//    delete(waveFileName);
//    delete(oggFileName);
//    delete(fullPath);
//}

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

    int index=0;//index of the SRT
    QString line;//current read line
    QString text;//subtitle text obtained from several reads.
    QString startTime;
    QString endTime;

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
            //write text on the table on row==index-1
            item(index-1,INDEX_SUBS_ORIGINAL_TEXT)->setText(text);
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
            //is simple text
            text.append(line);
        }
    }

    file->close();

    delete(inTextStream);
    delete(file);

    return true;
}

//TO DO: rewrite this
// Función Agregar_Archivo_SRT. Agrega texto de otro idioma a la tabla de subtítulos.
void Subtitles::addSRTArchive(QString Nombre_, QString Idioma_)
{

    eraseLanguage(Idioma_);
    int contador_campo = 0;
    int contador_fila = 0;
    int ultimo;

    int tabla_size = 0;
    QChar ultimochar;
    QFile file(Nombre_);
    QTextStream in(&file);
    QString celda1;
    QString celda2;
    QString line;
    QStringList list1;

    QString marca1("<>");
    marca1.insert(1,Idioma_);
    QString marca2("</>");
    marca2.insert(2,Idioma_);

    int lineas_texto;
    lineas_texto =1;
    ultimo = Nombre_.size();
    ultimochar = Nombre_.at(ultimo-1);

    if ((ultimochar=='t')&&(file.open(QFile::ReadOnly))) {

        while (!in.atEnd()) {
            line = in.readLine();

            if (contador_campo==1){
                list1 = line.split(" --> ");
                item(contador_fila,0)->setText(list1.at(0));
                item(contador_fila,1)->setText(list1.at(1));
            }

            if (contador_campo==2){
                celda1=item(contador_fila,2)->text();
                line.prepend(marca1);
                celda1.append(line);
                item(contador_fila,2)->setText(celda1);
            }

            if (contador_campo==3){
                celda1=item(contador_fila,2)->text();
                celda2=line;
                celda1.append(" ");
                celda1.append(celda2);
                celda1.append(marca2);
                item(contador_fila,2)->setText(celda1);
            }
            contador_campo++;

            if (line.isEmpty()==true){
                contador_campo=0;
                contador_fila++;
                if (contador_fila==(rowCount()-1)){
                    tabla_size = rowCount();
                    insertRow(rowCount());
                    for (int j = 0 ; j < 6 ; j++ )
                        setItem((rowCount()-1 ), j , new QTableWidgetItem );
                    tabla_size = rowCount();
                }
            }
        }
    }

    setRowCount(contador_fila);
    file.close();
}

//TO DO: rewrite this
// Función Eliminar_Idioma. Elimina un idioma de subtítulos.
void Subtitles::eraseLanguage(QString &Idioma_)
{
    QString subtitulos_aux;
    QString string_aux;
    QStringList lista1;
    QStringList lista2;

    QString marca1("<>");
    marca1.insert(1,Idioma_);
    QString marca2("</>");
    marca2.insert(2,Idioma_);

    subtitulos_aux = item(0,2)->text();
    if (subtitulos_aux.contains(marca2))
    {
        for (int row =0; row<rowCount(); row++)
        {
            if(!item(row,1)->text().isEmpty())
            {
                subtitulos_aux = item(row,2)->text();
                lista1 = subtitulos_aux.split(marca1);
                string_aux = lista1.at(1);
                lista2 = subtitulos_aux.split(marca2);
                string_aux = lista1.at(0);
                string_aux.append(lista2.at(1));
                item(row, 2)->setText(string_aux);
            }
        }
    }
}

//TO DO: rewrite this
// Slot cargar_audiodescriptor. Agrega un audiodescriptor a la tabla de subtítulos.
void Subtitles::loadAudiodescriptor(int indice_, QString nombre_,QString delay_)
{
    QString Audiodescriptores;
    QString string_aux;
    QStringList lista1;
    QStringList lista2;

    Audiodescriptores = item((indice_-1),5)->text();

    if (Audiodescriptores.contains(nombre_.left(4)))
    {
        lista1 = Audiodescriptores.split(nombre_.left(4));
        string_aux = lista1.at(1);
        lista2 = string_aux.split(nombre_.right(5));
        string_aux = lista1.at(0);
        string_aux.append(lista2.at(1));
        item((indice_ -1), 5)->setText(string_aux);
    }

    QString Celda_aux;
    Celda_aux = item((indice_ -1),5)->text();
    Celda_aux.append(nombre_);
    item((indice_ -1),5)->setText(Celda_aux);
    item((indice_ -1),4)->setText(delay_);
}

//TO DO: rewrite this
// Slot cargar_audiodoblado. Agrega audio doblado para un índice dado.
void Subtitles::loadAudioSubtitle(int indice_, QString nombre_)
{
    QString Audiodoblado;
    QString string_aux;
    QStringList lista1;
    QStringList lista2;

    Audiodoblado = item((indice_-1),3)->text();

    if (Audiodoblado.contains(nombre_.left(4)))
    {
        lista1 = Audiodoblado.split(nombre_.left(4));
        string_aux = lista1.at(1);
        lista2 = string_aux.split(nombre_.right(5));
        string_aux = lista1.at(0);
        string_aux.append(lista2.at(1));
        item((indice_ -1), 3)->setText(string_aux);
    }

    QString Celda_aux;
    Celda_aux = item((indice_ -1),3)->text();
    Celda_aux.append(nombre_);
    item((indice_ -1),3)->setText(Celda_aux);
}

QAction* Subtitles::getEditorAction()
{
    return action;
}
