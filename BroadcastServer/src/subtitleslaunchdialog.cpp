/******************************************************************************
 * STR2 Content Broadcast Server
 *
 * Developed by Oscar Lopes as the final project of the Multimedia Technologies
 * Master, and sponsored by CAIAC (caiac.uab.cat)
 *
 * (C)2011 [Universidad Autònoma de Barcelona]
 ******************************************************************************/

#include "subtitleslaunchdialog.h"

/******************************************************************************
 * This class controls the display of the subtitling table and also includes
 * the control logic, both for manual and automatic control.
 ******************************************************************************/

SubtitlesLaunchDialog::SubtitlesLaunchDialog(QWidget *parent) : QWidget(parent)
{

    rowTimeStampInMilis[NUM_OF_ROWS];

    //the current highlighted table line
    currentTableLine = 0;
    isSubtitleProjectLoaded = false;
    isAutoModeEnabled = false;
    isSessionPaused = false;

    //flag to check if the user resumes a paused session,
    //by selecting a new subtitle line of the table and pressing enter.
    isLineIndexChanged = false;

    xmlReader = new XMLProjectReader(this);

    initializeSubtitleData();
    createTimers();
    createSubtitleTable();
    createElements();

    enableElements(false);

    setWindowIcon(QIcon(":/icons/appicon.png"));
    setLayout(mainlayout);
}

/******************************************************************************
 * Controls the window's key event handlers.
 ******************************************************************************/
void SubtitlesLaunchDialog::keyPressEvent(QKeyEvent *event)
{
    if (!isSubtitleProjectLoaded)
    {
        return;
    }


    int indexRow;
    QMessageBox msgBox;

    switch (event->key()) {
    case Qt::Key_Bar:
        msgBox.setText("index row: "+QString::number(indexRow));
        msgBox.exec();
        //sendSubsButtonAction();
        update();
        break;
    case Qt::Key_Return:
        if(!isAutoModeEnabled){//Manual mode.
            sendSubsButtonAction();
            update();
            subtileTableWidget->setFocus();
        }
        else if(isAutoModeEnabled){
            isLineIndexChanged = true;
            pauseAutoTimer();
            startAutoTimer();
        }
        update();
        break;
        case Qt::Key_Down:
        update();
        break;
        case Qt::Key_Up:
        update();
        break;
        case Qt::Key_Insert:
        /* Send "shake" signal */
        broadCastHapticSignal();
        update();
        break;
        default:
        QWidget::keyPressEvent(event);
    }

}

/******************************************************************************
 * Initializes the base structure to hold the data.
 * Also it create the basic sockets to distribute the contents.
 ******************************************************************************/
void SubtitlesLaunchDialog::initializeSubtitleData()
{
    //initialize the structure that holds the subtitling content.
    subtitlesData = new SubtitleData;
    subtitlesData->currentLineIndex = 0;
    subtitlesData->catalanSubs = new QMap<int,SubtitleLineData*>();
    subtitlesData->englishSubs = new QMap<int,SubtitleLineData*>();
    subtitlesData->italianSubs = new QMap<int,SubtitleLineData*>();
    subtitlesData->spanishSubs = new QMap<int,SubtitleLineData*>();

    //initialize UDP content broadcast
    udpServerCatalan = new UDPContentServer(this,subtitlesData->catalanSubs,CATALAN_PORT,HAPTIC_PORT);
    udpServerEnglish = new UDPContentServer(this,subtitlesData->englishSubs,ENGLISH_PORT,HAPTIC_PORT);
    udpServerItalian = new UDPContentServer(this,subtitlesData->italianSubs,ITALIAN_PORT,HAPTIC_PORT);
    udpServerSpanish = new UDPContentServer(this,subtitlesData->spanishSubs,SPANISH_PORT,HAPTIC_PORT);

    udpLanguageServer = new UDPLanguagesServer(this,LANGUAGES_PORT);
}

/******************************************************************************
 * Initializes the Timer instances, which control the broadcast of the automatic
 * subtitles, global timer (aka display clock).
 ******************************************************************************/
void SubtitlesLaunchDialog::createTimers()
{
    currentSubLineIndex = 0;
    //isBroadCastPaused = false;
    totalTimeCount = 0; //total time in milis
    timerCount = new QTimer(this);

    connect(timerCount, SIGNAL(timeout()), this, SLOT(updateRunningTimer()));
    timerCount->setInterval(GLOBAL_TIMER_INTERVAL); //every 10 miliseconds

    sendInitSubsTimer = new QTimer(this);
    connect(sendInitSubsTimer, SIGNAL(timeout()), this, SLOT(sendSubsTimerTick()));

    sendEndSubsTimer = new QTimer(this);
    connect(sendEndSubsTimer, SIGNAL(timeout()), this, SLOT(sendEmptySubsTimerTick()));
}

/******************************************************************************
 * Basic GUI elements creation phase.
 ******************************************************************************/
void SubtitlesLaunchDialog::createElements()
{
    QFont font;
    font.setPointSize(12);
    font.setBold(true);
    autoMsm = new QLabel("Automatic controls:");
    manualMsm = new QLabel("Manual controls:");
    label_try = new QLabel("-Up/Down arrows: navigate table\n-Enter: send line\n-Insert: send haptic signal");
    lTimer = new QLabel("00:00:00,000");
    lTimer->setFont(font);
    totalLines = new QLabel("Last Line Sent: ");
    //    sep = new QLabel(this);
    //    sep->setFrameStyle(QFrame::HLine | QFrame::Plain);
    stat_bar = new QStatusBar();

    mainlayout = new QVBoxLayout(this);
    horizlayout = new QHBoxLayout(this);
    verticallayout = new QVBoxLayout(this);
    aLayout = new QGridLayout;
    agridGroupBox = new QGroupBox(tr("Automatic controls"));
    agridGroupBox->setFixedHeight(250);
    mgridGroupBox = new QGroupBox(tr("Manual controls"));
    langSelectionGroupBox = new QGroupBox(tr("Language Display"));

    horizlayout->addWidget(subtileTableWidget);
    horizlayout->addLayout(verticallayout);
    verticallayout->addWidget(agridGroupBox);
    verticallayout->addWidget(langSelectionGroupBox);
    verticallayout->addWidget(mgridGroupBox);

    sButton = new QPushButton(tr("Start"));
    QObject::connect(sButton, SIGNAL(clicked()),this, SLOT(startAutoTimer()));

    pButton = new QPushButton(tr("Pause"));
    QObject::connect(pButton, SIGNAL(clicked()),this, SLOT(pauseAutoTimer()));

    qButton = new QPushButton(tr("Stop"));
    QObject::connect(qButton, SIGNAL(clicked()),this, SLOT(stopAutoTimer()));

    aLayout->addWidget(sButton,0,0,Qt::AlignLeft);
    aLayout->addWidget(pButton,1,0,Qt::AlignLeft);
    aLayout->addWidget(qButton,2,0,Qt::AlignLeft);
    aLayout->addWidget(lTimer,1,1,Qt::AlignLeft);
    aLayout->addWidget(totalLines,4,0,Qt::AlignLeft);
    agridGroupBox->setLayout(aLayout);
    //agridGroupBox->setFixedWidth(200);

    mLayout = new QGridLayout;
    sendSubsButton = new QPushButton;
    sendSubsButton->setIcon(QIcon(":/icons/broadcastsubs.png"));
    sendSubsButton->setIconSize(QSize(65,65));
    sendSubsButton->setFixedSize(65,65);
    connect(sendSubsButton,SIGNAL(clicked()),this,SLOT(sendSubsButtonAction()));

    mLayout->addWidget(label_try,3,0,Qt::AlignCenter);
    mgridGroupBox->setLayout(mLayout);

    langHLayout = new QHBoxLayout;
    langsLabel = new QLabel(tr("Language"));
    langsComboBox = new QComboBox;
    langHLayout->addWidget(langsLabel);
    langHLayout->addWidget(langsComboBox);
    langSelectionGroupBox->setLayout(langHLayout);
    langSelectionGroupBox->setFixedHeight(100);

    mainlayout->addLayout(horizlayout);
    mainlayout->addWidget(stat_bar);

}

/******************************************************************************
 * Initialization of the GUI subtitles table.
 ******************************************************************************/
void SubtitlesLaunchDialog::createSubtitleTable()
{
    subtileTableWidget = new QTableWidget(this);
    subtileTableWidget->setStyleSheet("QTableView {selection-background-color: #FFFF00; selection-color: #0000FF;}");
    subtileTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    subtileTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    subtileTableWidget->setSortingEnabled(false);
    QStringList sl;
    sl << HEAD_ENTRY_TIME
            << HEAD_EXIT_TIME
            << HEAD_PAGE
            << HEAD_ACTOR
            << HEAD_ORIG_TEXT
            << HEAD_SUB_TEXT
            << HEAD_AUDIO_DESC
            << HEAD_COMMENT
            << HEAD_SUB_SOUND_FILE
            << HEAD_AD_SOUND_FILE;

    subtileTableWidget->setRowCount(NUM_OF_ROWS);
    subtileTableWidget->setColumnCount(sl.count());
    subtileTableWidget->setHorizontalHeaderLabels(sl);

    item_font = subtileTableWidget->font();
    item_font.setPixelSize(20);
    item_font.setFamily("Times");

    subtileTableWidget->setFont(item_font);

    subtileTableWidget->setColumnWidth(0,120);
    subtileTableWidget->setColumnWidth(1,120);
    subtileTableWidget->setColumnWidth(2,70);
    subtileTableWidget->setColumnWidth(3,100);
    subtileTableWidget->setColumnWidth(4,200);
    subtileTableWidget->setColumnWidth(5,200);
    subtileTableWidget->setColumnWidth(6,200);

    for(int i=0; i < subtileTableWidget->rowCount();i++)
    {
        subtileTableWidget->setRowHeight(i,70);

        for(int k=0; k < sl.count(); k++)
        {
            if(k == INDEX_SUBS_TRANSLATED_TEXT)
            {
                QTextEdit *textEdit = new QTextEdit;
                //                textEdit->setDisabled(true); not yet!
                textEdit->setFont(QFont("Arial", 12, QFont::Bold));
                textEdit->setAlignment(Qt::AlignCenter);
                textEdit->setPalette(QPalette(Qt::cyan));
                textEdit->setAutoFillBackground(true);
                textEdit->setLineWrapMode(QTextEdit::NoWrap);
                textEdit->setDisabled(true);
                subtileTableWidget->setCellWidget(i,k,textEdit);
            }
            else{
                QTableWidgetItem *item = new QTableWidgetItem;
                item->setFont(QFont("Arial", 12));
                item->setFlags(item->flags()&~Qt::ItemIsEditable);;
                subtileTableWidget->setItem(i,k,item);
            }
        }
    }

}

/******************************************************************************
 * Loads the contents of a STR2 project (stored in XML) to the subtitle
 * data structures.
 * Args: - projectPath - the global project path.
 *       - languageList - the languages we want to load (remenber this is
 *         obtained from the project.xml file previously loaded).
 ******************************************************************************/
void SubtitlesLaunchDialog::loadSubtitleDataFromXML(QString &projectPath,
                                                    QStringList &languageList)
{
    qDebug("ENTRY SubtitlesLaunchDialog::loadSubtitleDataFromXML");

    availableLanguagesList.clear();
    availableLanguagesList.append(languageList);

    for(int i=0; i<languageList.count();i++)
    {
        QString path = QString(projectPath);
        path.append("/"+languageList.at(i)+"/");
        path.append(languageList.at(i)+".xml");
        qDebug("XML Subtitle File: "+path.toAscii());

        if(languageList.at(i) == ENGLISH){
            xmlReader->readSubtitleXML(subtitlesData->englishSubs, path);
        }
        else if(languageList.at(i) == CATALAN){
            xmlReader->readSubtitleXML(subtitlesData->catalanSubs, path);
        }
        else if(languageList.at(i) == ITALIAN){
            xmlReader->readSubtitleXML(subtitlesData->italianSubs, path);
        }
        else if(languageList.at(i) == SPANISH){
            xmlReader->readSubtitleXML(subtitlesData->spanishSubs, path);
        }

    }

    qDebug("EXIT SubtitlesLaunchDialog::loadSubtitleDataFromXML");
}


/******************************************************************************
 * Defines what is the language that is going to be visible on the GUI for
 * monitoring.
 * Args: - lang : the languge to use as reference.
 ******************************************************************************/
void SubtitlesLaunchDialog::setVisibleSubtitleTable(QString &lang)
{
    qDebug("ENTRY SubtitlesLaunchDialog::setVisibleSubtitleTable lang = "+lang.toAscii());

    displayLanguage = lang;

    if(lang == ENGLISH)
    {
        loadDataToTableWidget(subtitlesData->englishSubs);
        qDebug("SubtitlesLaunchDialog::setVisibleSubtitleTable: loaded English");
    }
    else if(lang == CATALAN)
    {
        loadDataToTableWidget(subtitlesData->catalanSubs);
        qDebug("SubtitlesLaunchDialog::setVisibleSubtitleTable: loaded Catalan");
    }
    else if(lang == ITALIAN)
    {
        loadDataToTableWidget(subtitlesData->italianSubs);
        qDebug("SubtitlesLaunchDialog::setVisibleSubtitleTable: loaded Italian");
    }
    else if(lang == SPANISH)
    {
        loadDataToTableWidget(subtitlesData->spanishSubs);
        qDebug("SubtitlesLaunchDialog::setVisibleSubtitleTable: loaded Spanish");
    }
    else{
        return;
    }

    //Change the project loaded flag. So manual controls start working.
    isSubtitleProjectLoaded = true;

    qDebug("EXIT SubtitlesLaunchDialog::setVisibleSubtitleTable");

}

/******************************************************************************
 * Loads the data stored in the data structure to the GUI subtitle table.
 ******************************************************************************/
void SubtitlesLaunchDialog::loadDataToTableWidget(QMap<int,SubtitleLineData*> *subtitleDat)
{
    qDebug("ENTRY SubtitlesLaunchDialog::loadDataToTableWidget");

    qDebug("Number of entries: "+QString::number(subtitleDat->count()).toAscii());

    for(int i=0; i<subtitleDat->count() && i<subtileTableWidget->rowCount();i++)
    {
        SubtitleLineData *data = (*subtitleDat)[i+1];//add 1 to index because STR2 starts in index 1.

        const QString initTime  = QString(data->initTime->toAscii());
        const QString endTime   = QString(data->endTime->toAscii());
        const QString page      = QString(data->pageNumber->toAscii());
        const QString actor     = QString(data->actorName->toAscii());
        const QString origText  = QString(data->originalText->toAscii());
        const QString transText = QString(data->translatedSubtitle->toAscii());
        const QString audioDesc = QString(data->audioDesc->toAscii());
        const QString comment   = QString(data->comment->toAscii());
        const QString subSound  = QString(data->subtitleSoundFile->toAscii());
        const QString adSound   = QString(data->audioDescSoundFile->toAscii());

        /*qDebug("SubtitleData index = "+QString::number(i).toAscii()+
               "\n\tinitTime = "+initTime.toAscii()+
               "\n\tendTime = "+endTime.toAscii()+
               "\n\tpage = "+page.toAscii()+
               "\n\tactor = "+actor.toAscii()+
               "\n\torigText = "+origText.toAscii()+
               "\n\ttransText = "+transText.toAscii()+
               "\n\taudioDesc = "+audioDesc.toAscii()+
               "\n\tcomment = "+comment.toAscii()+
               "\n\tsubSound = "+subSound.toAscii()+
               "\n\tadSound = "+adSound.toAscii());*/

        if(data != 0){
            subtileTableWidget->item(i,0)->setText(initTime);
            subtileTableWidget->item(i,1)->setText(endTime);
            subtileTableWidget->item(i,2)->setText(page);
            subtileTableWidget->item(i,3)->setText(actor);
            subtileTableWidget->item(i,4)->setText(origText);
            QTextEdit *textEdit = (QTextEdit*)subtileTableWidget->cellWidget(i,INDEX_SUBS_TRANSLATED_TEXT);
            textEdit->setPlainText(transText);
            subtileTableWidget->item(i,6)->setText(audioDesc);
            subtileTableWidget->item(i,7)->setText(comment);
            subtileTableWidget->item(i,8)->setText(subSound);
            subtileTableWidget->item(i,9)->setText(adSound);
        }
    }

    langsComboBox->clear();
    langsComboBox->addItems(availableLanguagesList);
    setComboBoxToText(langsComboBox, displayLanguage);

    connect(langsComboBox,SIGNAL(activated(QString)),
            this,SLOT(languageComboBoxSelectionChanged(QString)));

    emit enableMainWindowMenus();

    qDebug("EXIT SubtitlesLaunchDialog::loadDataToTableWidget");
}


/******************************************************************************
 * Switches the value of the language selection combo box.
 * This is used during the initializatin when it chosen the display language.
 * Args: - combo : the combo box gui element.
 *       - text: the searched text.
 ******************************************************************************/
void SubtitlesLaunchDialog::setComboBoxToText(QComboBox *combo, QString &text)
{
    for(int i=0; i<combo->count(); i++)
    {
        if(combo->itemText(i) == text )
        {
            combo->setCurrentIndex(i);
        }
    }
}

/******************************************************************************
 * <SLOT>
 * This method is called when the combo box is changed to diplay another
 * language, so the subtitle table is updated accordingly.
 ******************************************************************************/
void SubtitlesLaunchDialog::languageComboBoxSelectionChanged(QString lang)
{
    qDebug("ENTRY SubtitlesLaunchDialog::languageComboBoxSelectionChanged lang="+lang.toAscii());

    if(displayLanguage != lang){
        setVisibleSubtitleTable( lang );
    }

    qDebug("EXIT SubtitlesLaunchDialog::languageComboBoxSelectionChanged");
}

/******************************************************************************
 * <Slot>
 * Triggers the broadcast of the next available subtitle line.
 ******************************************************************************/
void SubtitlesLaunchDialog::sendSubsButtonAction()
{
    int currentRow = subtileTableWidget->currentRow();

    broadCastContents(currentRow);

    totalLines->setText("Current Line: "+QString::number(currentRow+1));
}

/******************************************************************************
 * <Slot>
 * Starts the broadcast of the available languages list to the network.
 * This function should only be available after the project loading.
 ******************************************************************************/
void SubtitlesLaunchDialog::startBroadcastAction()
{
    qDebug("Entry SubtitlesLaunchDialog::startBroadcastAction");

    //start broadcasting the available languages...
    QString langs = availableLanguagesList.join(":");
    udpLanguageServer->startBroadCastingLanguages(langs);

    //enable GUI elements (buttons, comboBox, etc)
    enableElements(true);
    setManualMode(true);

    qDebug("EXIT SubtitlesLaunchDialog::startBroadcastAction");
}

/******************************************************************************
 * <Slot>
 * Stop the broadcast of the available languages list to the network.
 ******************************************************************************/
void SubtitlesLaunchDialog::stopBroadcastAction()
{
    qDebug("Entry SubtitlesLaunchDialog::stopBroadcastAction");

    enableElements(false);
    udpLanguageServer->stopBroadCastingLanguages();

    //TODO: add stop for the automatic mode... disable it's timer...

    qDebug("EXIT SubtitlesLaunchDialog::stopBroadcastAction");
}


/******************************************************************************
 * Broadcast to the network, the contents of a given language, if this language
 * is included on the available languages list.
 ******************************************************************************/
void SubtitlesLaunchDialog::broadCastContents(int index)
{
    qDebug("Entry SubtitlesLaunchDialog::broadCastContents index="+QString::number(index).toAscii());


    if(availableLanguagesList.contains(CATALAN))
    {
        udpServerCatalan->sendUDPMessage(index);
    }
    if(availableLanguagesList.contains(ENGLISH))
    {
        udpServerEnglish->sendUDPMessage(index);
    }
    if(availableLanguagesList.contains(ITALIAN))
    {
        udpServerItalian->sendUDPMessage(index);
    }
    if(availableLanguagesList.contains(SPANISH))
    {
        udpServerSpanish->sendUDPMessage(index);
    }

    qDebug("EXIT SubtitlesLaunchDialog::broadCastContents");
}

/******************************************************************************
 * Broadcast to the network, the command of a haptic signal (shaking) to each
 * of the network language UDP channels.
 ******************************************************************************/
void SubtitlesLaunchDialog::broadCastHapticSignal()
{
    qDebug("Entry SubtitlesLaunchDialog::broadCastHapticSignal ");

    if(availableLanguagesList.contains(CATALAN))
    {
        udpServerCatalan->sendHapticShakeSignal(1);
    }
    else if(availableLanguagesList.contains(ENGLISH))
    {
        udpServerEnglish->sendHapticShakeSignal(1);
    }
    else if(availableLanguagesList.contains(ITALIAN))
    {
        udpServerItalian->sendHapticShakeSignal(1);
    }
    else if(availableLanguagesList.contains(SPANISH))
    {
        udpServerSpanish->sendHapticShakeSignal(1);
    }

    qDebug("EXIT SubtitlesLaunchDialog::broadCastHapticSignal");
}

/******************************************************************************
 * Activates some elements of the GUI regarding the context of the application
 ******************************************************************************/
void SubtitlesLaunchDialog::enableElements(bool enable)
{
    mgridGroupBox->setEnabled(enable);
    agridGroupBox->setEnabled(enable);
    langSelectionGroupBox->setEnabled(enable);
}


/******************************************************************************
 * Activates the GUI elements to control the manual mode.
 ******************************************************************************/
void SubtitlesLaunchDialog::setManualMode(bool mode)
{
    mgridGroupBox->setEnabled(mode);
    agridGroupBox->setEnabled(!mode);

    isAutoModeEnabled = !mode;

}

bool SubtitlesLaunchDialog::checkTimeStampFormat(QString &timeStamp)
{
    if(timeStamp.isEmpty()){
        //nothing to do...
        return false;
    }

    const QString timeStampPattern = "[0-9]*:[0-5][0-9]:[0-5][0-9],[0-9][0-9][0-9]";

    QRegExp rx(timeStampPattern);
    Qt::CaseSensitivity cs = Qt::CaseInsensitive;
    rx.setCaseSensitivity(cs);
    rx.setMinimal(true);
    rx.setPatternSyntax(QRegExp::RegExp);

    return true;
}


/******************************************************************************
 * <Slot>
 * Updates the global running timer, that corresponds to the clock displayed
 * on the GUI.
 ******************************************************************************/
void SubtitlesLaunchDialog::updateRunningTimer()
{
    totalTimeCount += GLOBAL_TIMER_INTERVAL;

    setRunningTimer(totalTimeCount);
}

void SubtitlesLaunchDialog::setRunningTimer(int timeInMilis)
{
    if(timeInMilis<0){
        return;
    }


    int milisElapsed = clockTime.elapsed();

    totalTimeCount = timeInMilis;
    QString sTime = createFormattedTimeStamp(timeInMilis);
    lTimer->setText(sTime);
}

/******************************************************************************
 * Builds a timestamp string (Hh:Mm:ss,mmm) from the miliseconds.
 ******************************************************************************/
QString SubtitlesLaunchDialog::createFormattedTimeStamp(int timeMilis)
{
    int hours, minutes, minutesDelta,seconds, secondsDelta, milis, milisDelta;
    QString sHours, sMinutes, sSeconds, sMilis;

    milisDelta = timeMilis%1000;
    seconds = timeMilis/1000;
    secondsDelta = seconds%60;
    minutes = seconds/60;
    minutesDelta = minutes%60;
    hours = minutes/60;

    sHours   = QString::number(hours);
    sMinutes = QString::number(minutesDelta);
    sSeconds = QString::number(secondsDelta);
    sMilis   = QString::number(milisDelta);

    //timestamp example: 00:01:10,250
    if(sHours.length()==1){
        sHours.prepend("0");
    }

    if(sMinutes.length()==1){
        sMinutes.prepend("0");
    }

    if(sSeconds.length()==1){
        sSeconds.prepend("0");
    }

    if(sMilis.length()==1){
        sMilis.prepend("00");
    }
    else if(sMilis.length()==2){
        sMilis.prepend("0");
    }

    QString timeStamp;
    timeStamp.append(sHours)
            .append(":")
            .append(sMinutes)
            .append(":")
            .append(sSeconds)
            .append(",")
            .append(sMilis);


    return timeStamp;

}

/******************************************************************************
 * Converts a timestamp string (Hh:Mm:ss,mmm) to miliseconds.
 * Args: - timeStamp: a string with the clock string (HH:Mm:ss,uuu)
 ******************************************************************************/
int SubtitlesLaunchDialog::convertTimeStampToMilis(QString timeStamp)
{
    if(timeStamp.isNull() || timeStamp.isEmpty() || !checkTimeStampFormat(timeStamp))
    {
        return 0;
    }

    // From "Hh:Mm:ss,uuu" we obtain ["Hh", "Mm" ,"ss,uuu" ]
    QStringList timeComponents = timeStamp.split(":");
    int hours = ((QString)timeComponents.at(0)).toInt();
    int minutes = ((QString)timeComponents.at(1)).toInt();

    // From "ss,uuu" we obtain ["ss","uuu" ]
    QStringList secondsMilis = ((QString)timeComponents.at(2)).split(",");
    int seconds = ((QString)secondsMilis.at(0)).toInt();
    int milis = ((QString)secondsMilis.at(1)).toInt();

    int timeInMilis = milis + 1000*(seconds + 60*(minutes + 60*hours));

    return timeInMilis;
}

/******************************************************************************
 * Loads the time stamps, from the data currently loaded on the subtitle
 * table widget.
 * IMPORTANT: It is assumed that ALL the languages have the same timestamps
 *            for each correspondent subtitle line. Since we're retrieving the
 *            timestamps form one of them. However this assumption must be
 *            enforced by the content generator.
 ******************************************************************************/
void SubtitlesLaunchDialog::loadTimestampsLaunchTicks()
{
    qDebug("ENTRY loadTimestampsLaunchTicks");
    for(int i=0; i<NUM_OF_ROWS; i++)
    {
        const QString initTimeStamp = subtileTableWidget->item(i,0)->text();
        const QString endTimeStamp = subtileTableWidget->item(i,1)->text();

        int iniTimeStampInMils = convertTimeStampToMilis(initTimeStamp);
        int endTimeStampInMils = convertTimeStampToMilis(endTimeStamp);

        rowTimeStampInMilis[i] = iniTimeStampInMils;
        rowTimeStampEndInMilis[i] = endTimeStampInMils;
        qDebug("\t<Index = "+QString::number(i).toAscii()+"> "+
               initTimeStamp.toAscii()+" -> "+ endTimeStamp.toAscii());

    }

    qDebug("EXIT loadTimestampsLaunchTicks");
}

/******************************************************************************
 * Starts the auto timer clock, which enables the clock that is visible in the
 * GUI and the timer that controls the subtitle launching.
 *
 * Important note:
 *              Each subtitle line has to be scheduled separately and
 *                 sequentially, so if you have S1, and S2 subtitles and
 *                 t1, t2 the correspondent timestamp in which they must be
 *                 broadcasted, the timer of S2, must be programmed to launch it
 *                 on t2-t1 (in milis).
 *              Qt timers are programmed by time intervals, and not in respect
 *                 to a Global clock.
 ******************************************************************************/
void SubtitlesLaunchDialog::startAutoTimer()
{
    qDebug("ENTRY startAutoTimer");

    if(currentSubLineIndex>NUM_OF_ROWS)
    {
        return;//just in case;
    }

    if(subtileTableWidget->item(subtileTableWidget->currentRow(),0)->text().isEmpty())
    {
        //If we reach a line with no start timestamp it's assumed that we've
        //reached the end of the session, since all sequential subtitle lines
        //must have one.
        QMessageBox msgBox;
        msgBox.setText("The does not have initial timestamp!");
        msgBox.setModal(true);
        msgBox.exec();

        return;
    }

    if(!isSessionPaused){
        sendInitSubsTimer->setInterval(calculateLaunchTimeInterval(currentSubLineIndex));
        sendEndSubsTimer->setInterval(calculateRemoveTimeInterval(currentSubLineIndex));

    }
    else if(isSessionPaused){//session IS paused.
        if(isLineIndexChanged){
            //configure the new line to broadcast
            currentSubLineIndex = subtileTableWidget->currentRow();
            resetTimerCounterForNewStart(currentSubLineIndex);

            //send it NOW!
            sendInitSubsTimer->setInterval(0);
        }
//        else{ //resume session from where was stopeed.
//            //session was previouly paused so we must calculate the time diff
//            //from the Global time counter to the NEXT time stamp.
//            int milisOfNext = convertTimeStampToMilis(subtileTableWidget->item(currentSubLineIndex+1,0)->text());
//            sendSubsTimer->setInterval(milisOfNext-totalTimeCount);
//        }
        isSessionPaused = false;
    }
    timerCount->start();
    sendInitSubsTimer->start();
    sendEndSubsTimer->start();
    qDebug("EXIT startAutoTimer");
}


/******************************************************************************
 * <Slot>
 * Pauses the automatic broadcast launch, by stoping both the global timer
 * (timerCount) the one that holds the Gui's clock, and the subtitle launching
 * timer (sendSubsTimer) programmed with the time differences between two
 * subtitles.
 ******************************************************************************/
void SubtitlesLaunchDialog::pauseAutoTimer()
{
    //isBroadCastPaused = true;
    timerCount->stop();
    sendInitSubsTimer->stop();
    sendEndSubsTimer->stop();

    isSessionPaused = true;

}

/******************************************************************************
 * <Slot>
 * Pauses the automatic broadcast launch.
 * Similar as pause but resets the time counter to 0, and to  the first subtitle.
 ******************************************************************************/
void SubtitlesLaunchDialog::stopAutoTimer()
{
    //currentSubLineIndex=0;
    timerCount->stop();
    sendInitSubsTimer->stop();
    sendEndSubsTimer->stop();
    resetTimerCounter();
    currentSubLineIndex = 0;
    isSessionPaused = false;

}

/******************************************************************************
 * Broadcasts the subtitles under a timer tick (aka by the end of the time
 * interval defined).
 ******************************************************************************/
void SubtitlesLaunchDialog::sendSubsTimerTick()
{
    qDebug("ENTRY sendSubsTimerTick");

    //send the line on the index...
    broadCastContents(currentSubLineIndex);

    //update label
    totalLines->setText(QString("Last Line Sent: "+QString::number(currentSubLineIndex+1).toAscii()));

    //this ensures that the current line is visible in the table...
    subtileTableWidget->scrollToItem(subtileTableWidget->item(currentSubLineIndex,0));

    currentSubLineIndex++;
    //configure the timer for the next "tick"
    if(currentSubLineIndex<NUM_OF_ROWS)//just in case...
    {
        if(rowTimeStampInMilis[currentSubLineIndex] > 0){
            sendInitSubsTimer->stop();

            /*
             * Calculate the differences between two consecutive timeStamps(in milis)
             * This differences will be used to configure the QTimer that controls the
             * the launching of the subtitles.
             */
            int intervalToNext = calculateLaunchTimeInterval(currentSubLineIndex);

            if(intervalToNext != 0){
                //If intervalToNext==0
                //This means that the timestamps are empty and we've reached
                //the end of the session

                sendInitSubsTimer->setInterval(intervalToNext);
                qDebug("\tTimeStamp difference: "+ QString::number(intervalToNext).toAscii()+" ms");
                sendInitSubsTimer->start();
            }
        }
    }

    //Peek next line, to see if we've reached the tragic end...
    if(subtileTableWidget->item(currentSubLineIndex+1,0)->text().isEmpty())
    {
        pauseAutoTimer();

        QMessageBox msgBox;
        msgBox.setText("The session has ended!");
        msgBox.setModal(true);
        msgBox.exec();
    }

    qDebug("EXIT sendSubsTimerTick");
}

/******************************************************************************
 * Broadcasts the subtitles under a timer tick (aka by the end of the time
 * interval defined).
 ******************************************************************************/
void SubtitlesLaunchDialog::sendEmptySubsTimerTick()
{
    qDebug("ENTRY sendSubsTimerTick");

    //send the empty line to clear out mobile
    broadCastContents(-1);

    //configure the timer for the next "tick"
    if(currentSubLineIndex<NUM_OF_ROWS)//just in case...
    {
        if(rowTimeStampEndInMilis[currentSubLineIndex] > 0){
            sendEndSubsTimer->stop();

            /*
             * Calculate the differences between two consecutive timeStamps(in milis)
             * This differences will be used to configure the QTimer that controls the
             * the launching of the subtitles.
             */
            int intervalToNext = calculateRemoveTimeInterval(currentSubLineIndex);

            if(intervalToNext != 0){
                //If intervalToNext==0
                //This means that the timestamps are empty and we've reached
                //the end of the session

                sendEndSubsTimer->setInterval(intervalToNext);
                qDebug("\tTimeStamp difference: "+ QString::number(intervalToNext).toAscii()+" ms");
                sendEndSubsTimer->start();
            }
        }
    }

    //Peek next line, to see if we've reached the tragic end...
    if(subtileTableWidget->item(currentSubLineIndex+1,0)->text().isEmpty())
    {
        pauseAutoTimer();

        QMessageBox msgBox;
        msgBox.setText("The session has ended!");
        msgBox.setModal(true);
        msgBox.exec();
    }

    qDebug("EXIT sendSubsTimerTick");
}

/******************************************************************************
 * Resets the GUI timer clock to zero, and also the associated milis counter.
 ******************************************************************************/
void SubtitlesLaunchDialog::resetTimerCounter()
{
    totalTimeCount = 0;
    lTimer->setText("00:00:00.000");
}

/******************************************************************************
 * Resets the global counter to a new start timestamp, updating the visible
 * clock on the GUI, and the internal miliseconds counter.
 *****************************************************************************/
void SubtitlesLaunchDialog::resetTimerCounterForNewStart(int index)
{
    if(index <0 || index > NUM_OF_ROWS){
        return;
    }

    QString timeStamp = subtileTableWidget->item(index,0)->text();
    if(timeStamp.isEmpty()){
        return;
    }

    int milis = convertTimeStampToMilis(timeStamp);

    clockTime = QTime::fromString(timeStamp,"hh:mm:ss,zzz");
    clockTime.start();

    setRunningTimer(milis);
}

/******************************************************************************
 * Calculates the time interval between T[baseIndex] and T[baseIndex-1]
 ******************************************************************************/
int SubtitlesLaunchDialog::calculateLaunchTimeInterval(int baseIndex)
{
    if(baseIndex<0){
        return 0;
    }

    if(baseIndex==0){
        return rowTimeStampInMilis[0];
    }
    else{
        return rowTimeStampInMilis[baseIndex]-rowTimeStampInMilis[baseIndex-1];
    }
}

/******************************************************************************
 * Calculates the end time interval between T[baseIndex] and T[baseIndex-1]
 ******************************************************************************/
int SubtitlesLaunchDialog::calculateRemoveTimeInterval(int baseIndex)
{
    if(baseIndex<0){
        return 0;
    }

    if(baseIndex==0){
        return rowTimeStampEndInMilis[0];
    }
    else{
        return rowTimeStampEndInMilis[baseIndex]-rowTimeStampEndInMilis[baseIndex-1];
    }
}

SubtitlesLaunchDialog::~SubtitlesLaunchDialog()
{
    udpServerCatalan->closeSocket();
    delete udpServerCatalan;
    udpServerEnglish->closeSocket();
    delete udpServerEnglish;
    udpServerItalian->closeSocket();
    delete udpServerItalian;
    udpServerSpanish->closeSocket();
    delete udpServerSpanish;
    udpLanguageServer->stopBroadCastingLanguages();
    delete udpLanguageServer;

}
