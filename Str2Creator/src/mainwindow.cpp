#include "mainwindow.h"

#define BUFFER_SIZE 4096

mainwindow::mainwindow()
{
    setWindowTitle(APP_NAME);
    setWindowIcon(QIcon(":/icons/appicon.png"));
    resize(1024,768);

    createElements();

    createActions();
    createMenus();
    createMainToolbar();
    createStatusbar();

    isProjectOpened=false;

    projectConfigDialog = new ProjectSetUpConfig(this);
//    connect(projectConfigDialog, SIGNAL(setProjectPath(QString&)),
//            this, SLOT(setProjectPath(QString&)));
//    connect(projectConfigDialog, SIGNAL(setProjectName(QString&)),
//            this, SLOT(setProjectName(QString&)));
    connect(projectConfigDialog, SIGNAL(setProjectProperties(QMap<QString,QString>&)),
            this, SLOT(setProjectProperties(QMap<QString,QString>&)));
    connect(projectConfigDialog,SIGNAL(generateGUI()),
            this, SLOT(generateGUI()));

    createProjectTree();
    initializeSupportedLanguagesMap();

    setCentralWidget(mdiArea);

    //  audio = new QAudioInput();
    //    grabadora = 0;
    //    cargador = 0;
    //    reproductor = 0;

    //    QDir def_dir;
    //    QString directorio_default;
    //    directorio_default = def_dir.absolutePath();
    //    directorio_default.append("buffer_pistas_audio");
    //    definir_directorios(directorio_default);
    //    inicializar_buffer();

    langAudioGen = new AudioGeneratorConfig(this);
    connect(langAudioGen, SIGNAL(startTTSAudioGeneration(QStringList&)), this, SLOT(startTTSAudioGeneration(QStringList&)));

}

/******************************************************************************
 * Initializes some elements to be later used.
 ******************************************************************************/
void mainwindow::createElements()
{
    mdiArea = new QMdiArea(this);
    audioRecorder = new AudioRecorder(this, this);
    aboutDialog = new About(this);
}

/******************************************************************************
 * Creates all the actions to be used within the menus and toolbars.
 * Also establish the connections.
 ******************************************************************************/
void mainwindow::createActions()
{
    newProjectAction = new QAction(tr("&New Project..."), this);
    newProjectAction->setShortcut(QKeySequence::New);
    newProjectAction->setIcon(QIcon(":/icons/document-new.png"));
    newProjectAction->setStatusTip(tr("Creates a new subtitle project"));
    connect(newProjectAction, SIGNAL(triggered()), this, SLOT(newProject()));

    openProjectAction = new QAction(tr("&Open Project..."), this);
    openProjectAction->setShortcut(QKeySequence::Open);
    openProjectAction->setIcon(QIcon(":/icons/document-open.png"));
    openProjectAction->setStatusTip(tr("Open an existent project"));
    connect(openProjectAction, SIGNAL(triggered()), this, SLOT(openProject()));

    closeProjectAction = new QAction(tr("Close &Project..."), this);
    closeProjectAction->setShortcut(QKeySequence::Close);
    closeProjectAction->setIcon(QIcon(":/icons/close.png"));
    closeProjectAction->setStatusTip(tr("Closes current project"));
    closeProjectAction->setDisabled(true);
    connect(closeProjectAction, SIGNAL(triggered()), this, SLOT(closeProject()));

    saveProjectAction = new QAction(tr("&Save Project..."), this);
    saveProjectAction->setShortcut(QKeySequence::Save);
    saveProjectAction->setIcon(QIcon(":/icons/document-save.png"));
    saveProjectAction->setStatusTip(tr("Save current project"));
    saveProjectAction->setDisabled(true);
    connect(saveProjectAction, SIGNAL(triggered()), this, SLOT(saveProject()));

    closeAllEditorsAction = new QAction(tr("Close &All"), this);
    closeAllEditorsAction->setStatusTip(tr("Closes all language editors"));
    closeAllEditorsAction->setDisabled(true);
    connect(closeAllEditorsAction, SIGNAL(triggered()), this, SLOT(closeAllEditors()));

    closeCurrentEditorAction = new QAction(tr("Close current &window"), this);
    closeCurrentEditorAction->setStatusTip(tr("Closes current language editors"));
    closeCurrentEditorAction->setDisabled(true);
    connect(closeCurrentEditorAction, SIGNAL(triggered()), this, SLOT(closeCurrentEditor()));

    cascadeSubWindowsAction = new QAction(tr("C&ascade windows"),this);
    cascadeSubWindowsAction->setStatusTip(tr("Cascade all windows"));
    cascadeSubWindowsAction->setDisabled(true);
    connect(cascadeSubWindowsAction, SIGNAL(triggered()), this, SLOT(cascadeWindows()));

    tileSubWindowsAction = new QAction(tr("&Tile windows"), this);
    tileSubWindowsAction->setStatusTip(tr("Tile all windows"));
    tileSubWindowsAction->setDisabled(true);
    connect(tileSubWindowsAction, SIGNAL(triggered()), this, SLOT(tileWindows()));

    showProjectDockPanelAction = new QAction(tr("&Project Properties"), this);
    showProjectDockPanelAction->setIcon(QIcon(":/icons/properties.png"));
    showProjectDockPanelAction->setStatusTip(tr("Show project Properties dock panel"));
    showProjectDockPanelAction->setDisabled(true);
    connect(showProjectDockPanelAction, SIGNAL(triggered()), this, SLOT(showProjectPropertiesDock()));

    showAudioRecorderDockAction = new QAction(tr("&Audio Recorder"), this);
    showAudioRecorderDockAction->setIcon(QIcon(":/icons/music_recorder.png"));
    showAudioRecorderDockAction->setStatusTip(tr("Show Audio Recorder Panel"));
    showAudioRecorderDockAction->setDisabled(true);
    connect(showAudioRecorderDockAction, SIGNAL(triggered()), audioRecorder, SLOT(showDockableAudioRecorder()));

    exportProjectAction = new QAction(tr("&Export Project..."), this);
    exportProjectAction->setIcon(QIcon(":/icons/project_export.png"));
    exportProjectAction->setStatusTip(tr("Export current project to a single file"));
    exportProjectAction->setDisabled(true);
    connect(exportProjectAction, SIGNAL(triggered()), this, SLOT(exportProject()));

    importProjectAction = new QAction(tr("&Import Project..."), this);
    importProjectAction->setIcon(QIcon(":/icons/project_import.png"));
    importProjectAction->setStatusTip(tr("Import project from file"));
    importProjectAction->setDisabled(true);
    connect(importProjectAction, SIGNAL(triggered()), this, SLOT(importProject()));

    generateAudioDescFromTTS = new QAction(tr("&Generate Sound Files..."),this);
    generateAudioDescFromTTS->setIcon(QIcon(":/icons/audio.png"));
    generateAudioDescFromTTS->setStatusTip(tr("Generate sound files from translated subtitles & audio descriptions using TTS system"));
    generateAudioDescFromTTS->setDisabled(true);
    connect(generateAudioDescFromTTS, SIGNAL(triggered()), this, SLOT(showMenuTTSConfig()));

    loadSrtFileAction = new QAction(tr("&Load SRT File..."), this);
    loadSrtFileAction->setIcon(QIcon(":/icons/Importar.png"));
    loadSrtFileAction->setStatusTip(tr("Loads the SRT file content to the current active language window"));
    loadSrtFileAction->setDisabled(true);
    connect(loadSrtFileAction, SIGNAL(triggered()), this, SLOT(loadSrtFileToActiveSubtitle()));

    aboutQTAction = new QAction(tr("About QT &Framework"), this);
    connect(aboutQTAction, SIGNAL(triggered()), this, SLOT(showQTFrameworkInfo()) );

    aboutApplicationAction = new QAction(tr("&About..."), this);
    connect(aboutApplicationAction, SIGNAL(triggered()), this, SLOT(showAboutInfo()));
}

/******************************************************************************
 * Creates toolbars for the GUI
 ******************************************************************************/
void mainwindow::createMainToolbar()
{
    fileToolbar = addToolBar(tr("&File"));
    fileToolbar->addAction(newProjectAction);
    fileToolbar->addAction(openProjectAction);
    fileToolbar->addAction(saveProjectAction);
    fileToolbar->addAction(closeProjectAction);

    toolsToolbar = addToolBar(tr("Tools"));
    toolsToolbar->addAction(importProjectAction);
    toolsToolbar->addAction(exportProjectAction);
    toolsToolbar->addSeparator();
    toolsToolbar->addAction(showProjectDockPanelAction);
    toolsToolbar->addAction(showAudioRecorderDockAction);
    toolsToolbar->addSeparator();
    toolsToolbar->addAction(loadSrtFileAction);
    toolsToolbar->addAction(generateAudioDescFromTTS);

}

/******************************************************************************
 * Creates all the menu items.
 ******************************************************************************/
void mainwindow::createMenus()
{
    menuArchive = menuBar()->addMenu(tr("&File"));
    menuArchive->addAction(newProjectAction);
    menuArchive->addAction(openProjectAction);
    menuArchive->addAction(saveProjectAction);
    menuArchive->addAction(closeProjectAction);

    menuTools = menuBar()->addMenu(tr("&Tools"));
    menuTools->addAction(loadSrtFileAction);
    menuTools->addSeparator();
    menuTools->addAction(importProjectAction);
    menuTools->addAction(exportProjectAction);
    menuTools->addSeparator();
    menuTools->addAction(generateAudioDescFromTTS);

    menuWindows = menuBar()->addMenu(tr("&Windows"));
    menuWindows->addAction(closeCurrentEditorAction);
    menuWindows->addAction(closeAllEditorsAction);
    menuWindows->addSeparator();
    menuWindows->addAction(cascadeSubWindowsAction);
    menuWindows->addAction(tileSubWindowsAction);
    menuWindows->addSeparator();
    menuWindows->addAction(showProjectDockPanelAction);
    menuWindows->addAction(showAudioRecorderDockAction);
    menuWindows->addSeparator();

    menuHelp = menuBar()->addMenu(tr("&Help"));
    menuHelp->addAction(aboutQTAction);
    menuHelp->addAction(aboutApplicationAction);

}

/******************************************************************************
 * Creates the status bar.
 ******************************************************************************/
void mainwindow::createStatusbar()
{
    locationLabel = new QLabel(this);
    locationLabel->setAlignment(Qt::AlignHCenter);
    locationLabel->setMinimumSize(locationLabel->sizeHint());

    formulaLabel = new QLabel(this);
    formulaLabel->setIndent(3);
    statusBar()->addWidget(locationLabel);
    statusBar()->addWidget(formulaLabel, 1);
}

/******************************************************************************
 * Update the status bar.
 ******************************************************************************/
void mainwindow::updateStatusBar(){
    QString msg = "Project Base Path: ";
    msg.append(projectBasePath);
    locationLabel->setText(msg);
}


/*
 * Slot handlers implementation.
 */

/******************************************************************************
 * <SLOT>
 * Creates a new project.
 ******************************************************************************/
void mainwindow::newProject()
{
    projectConfigDialog->show();
}

/******************************************************************************
 * <SLOT>
 * Closes a project.
 ******************************************************************************/
bool mainwindow::closeProject()
{
    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setText("¿ Are you sure you want to close the current project ?");
    msgBox->setStandardButtons(QMessageBox::Ok| QMessageBox::Cancel);
    int answer = msgBox->exec();

    switch (answer) {
    case QMessageBox::Ok:
        {
            isProjectOpened = false;
            closeAllDockablePanels();
            mdiArea->closeAllSubWindows();
            disableActions(true);
            initializeSupportedLanguagesMap();
            return true;
        }
    case QMessageBox::Cancel:
        {
            return false;
        }
    }
}

/******************************************************************************
 * <SLOT>
 * Saves the current project information to the correspondent XML's associated
 * to each subtitle window.
 ******************************************************************************/
void mainwindow::saveProject()
{
    QProgressDialog *progressDiag = new QProgressDialog(this);
    progressDiag->setWindowTitle(tr("Save Project"));
    progressDiag->setModal(true);
    //    progressDiag->setAutoClose(true);
    progressDiag->setCancelButton(0);//hide it...
    QLabel *progressLabel = new QLabel();
    progressLabel->setAlignment(Qt::AlignCenter);
    progressDiag->setLabel(progressLabel);
    progressDiag->setFixedSize(350,100);
    progressDiag->setMinimum(0);
    progressDiag->setMaximum(supportedLangagesMap.count());
    progressDiag->show();

    int steps = 0;//current progress step

    QList<QString> langs = supportedLangagesMap.keys();

    for(int i=0; i< langs.size(); i++){
        progressLabel->setText("Saving language subtitles...");
        progressDiag->show();

        QString lang = langs.at(i);
        //check if the language is currently supported
        if(supportedLangagesMap[lang]->isSupported){
            //export the XML file from table.
            QString *pbd = new QString(projectBasePath);
            QString languageDir = pbd->append("/").append(lang);
            QDir *dir = new QDir();
            if(!dir->exists(languageDir)){
                //create language base directory
                dir->mkdir(languageDir);
            }
            QString str = "Saving ";
            str.append(lang).append(" language subtitle table...");
            progressLabel->setText(str);

            supportedLangagesMap[lang]->sub->saveSRT2Archive(languageDir, lang);

            progressDiag->setValue(++steps);

        }
    }

    delete progressLabel;
    delete progressDiag;

}

/******************************************************************************
 * <SLOT>
 * Opens an already existent project.
 ******************************************************************************/
void mainwindow::openProject()
{

    if(isProjectOpened)
    {
        if(!closeProject()){
            //A project is opened and theb user didn't close the current one
            return;
        }
    }

    qDebug("ENTRY mainwindow::openProject");

    QString fileName = QFileDialog::getOpenFileName(
            this,
            tr("Open Project"),
            QDir::currentPath(),
            tr("STR2 Project (project.xml)") );

    qDebug("\topened project file = "+fileName.toAscii());

    if(fileName.count()==0){
        //the user pressed cancel... nothing to do
        return;
    }

    QMap<QString,QString> retrievedProps;

    if(fileName!=0 && !fileName.isEmpty())
    {
        XMLProjectExport *xmlExport = new XMLProjectExport(this);
        retrievedProps = xmlExport->readProjectDefinitionXML(fileName);
    }

    QMap<QString,QString> projectProps = QMap<QString,QString>();

    QFileInfo info(fileName);
    QString projectPath = info.canonicalPath();

    projectProps.insert(PROJ_CONFIG_NAME,retrievedProps[STR_PROJECTNAME]);
    projectProps.insert(PROJ_CONFIG_DESC,retrievedProps[STR_PROJECTDESC]);
    projectProps.insert(PROJ_CONFIG_LANGS,retrievedProps[STR_LANGUAGES]);
    projectProps.insert(PROJ_CONFIG_BASE_PATH,projectPath);

    qDebug("\tproject properties:");
    qDebug("\t-project name = "+retrievedProps[STR_PROJECTNAME].toAscii());
    qDebug("\t-project description = "+retrievedProps[STR_PROJECTDESC].toAscii());
    qDebug("\t-project languages = "+retrievedProps[STR_LANGUAGES].toAscii());
    qDebug("\t-project base path = "+projectPath.toAscii());



    setProjectProperties(projectProps);

    generateGUI();

    QString pp(projectPath);
    QStringList langs(projectProps[PROJ_CONFIG_LANGS].split(":"));
    loadXMLDataToSubtitlesTables(pp, langs);

    qDebug("ENTRY mainwindow::openProject");

}

void mainwindow::loadXMLDataToSubtitlesTables(QString &basePath,
                                              QStringList &languages){

    XMLProjectExport *xmlReader = new XMLProjectExport(this);

    for(int i=0; i<languages.count(); i++)
    {
        QString lang = languages.at(i);
        QString filePath(basePath);
        filePath.append("/"+ lang +"/"+lang+".xml");
        Subtitles *sub = supportedLangagesMap[languages.at(i)]->sub;
        xmlReader->readSubtitleXML(sub, filePath);
    }

}

/******************************************************************************
 * <SLOT>
 * Exports a project to a zip file.
 ******************************************************************************/
void mainwindow::exportProject()
{
    QLabel *progressLabel = new QLabel();
    progressLabel->setAlignment(Qt::AlignCenter);
    progressLabel->setText("Exporting project...");

    QProgressDialog *progressDialog = new QProgressDialog(this);
    progressDialog->setLabel(progressLabel);
    progressDialog->setWindowTitle("STR2 Export");
    progressDialog->setModal(true);
    progressDialog->setMinimum(0);
    progressDialog->setMaximum(0);
    progressDialog->setAutoClose(true);
    progressDialog->setCancelButton(0);
    progressDialog->setFixedSize(350,100);

    QString file = QFileDialog::getSaveFileName(
            this,
            tr("Choose STR2 export file"),
            QDir::currentPath(),
            tr("STR2 Project Export (*.str2)"));

    QString *zipFile = new QString(file);

    if(zipFile->isEmpty()){
        //if user pressed cancel...
        return;
    }

    progressDialog->show();
    //progressDialog->exec();

    /*
     * If user writes the right extension don't append it again.
     * This is required because the zip utility adds  the "zip"
     * extension if none is declared.
     */
    if(!zipFile->endsWith(STR2_EXPORTFILE_EXT)){
        zipFile->append(STR2_EXPORTFILE_EXT);
    }

    Utils::zipDirectory("zip", projectBasePath, *zipFile);


    progressDialog->close();

    //    delete progressDialog;
    //    delete progressLabel;
}

/******************************************************************************
 * <SLOT>
 * Imports a project.
 ******************************************************************************/
void mainwindow::importProject()
{
    //TODO
}

/******************************************************************************
 * Enables/disables some actions from menus and toolbars.
 * Args:
 * - bool activate: if true disables the actions, otherwise activates them.
 ******************************************************************************/
void mainwindow::disableActions(bool activate){
    newProjectAction->setDisabled(!activate);
    openProjectAction->setDisabled(!activate);
    closeAllEditorsAction->setDisabled(activate);
    closeCurrentEditorAction->setDisabled(activate);
    tileSubWindowsAction->setDisabled(activate);
    cascadeSubWindowsAction->setDisabled(activate);
    exportProjectAction->setDisabled(activate);
    importProjectAction->setDisabled(activate);
    saveProjectAction->setDisabled(activate);
    generateAudioDescFromTTS->setDisabled(activate);
    loadSrtFileAction->setDisabled(activate);
    showAudioRecorderDockAction->setDisabled(activate);
    showProjectDockPanelAction->setDisabled(activate);
    closeProjectAction->setDisabled(activate);
}

/******************************************************************************
 * Enables/disables some actions from menus and toolbars.
 * Args:
 * - bool activate: if true disables the actions, otherwise activates them.
 ******************************************************************************/
void mainwindow::closeAllDockablePanels()
{
    //used when closing a projec...
    projectPropertiesDockWidget->hide();
    audioRecorder->hideDockableAudioRecorder();

}


//SLOT
void  mainwindow::setProjectProperties(QMap<QString,QString> &properties)
{
    projectProperties = properties;

    projectName = properties[PROJ_CONFIG_NAME];
    projectDescription = properties[PROJ_CONFIG_DESC];
    projectBasePath = properties[PROJ_CONFIG_BASE_PATH];

    setSupportedLanguages(properties[PROJ_CONFIG_LANGS].split(":"));
}

void mainwindow::setSupportedLanguages(QStringList languages)
{
    QProgressDialog pd(this);
    QLabel lab("Creating Languages...");
    pd.setModal(true);
    pd.setCancelButton(0);
    pd.setWindowTitle("Project Setup");
    pd.setLabel(&lab);
    pd.setMinimum(0);
    pd.setMaximum(languages.count()+1);
    pd.setFixedSize(200,100);
    pd.show();

    supportedProjectLanguages.clear();
    supportedProjectLanguages.append(languages);

    int i;
    for(i=0; i<languages.count(); i++)
    {
        pd.setValue(i);
        qApp->processEvents();
        supportedLangagesMap[languages.at(i)]->isSupported = true;

        //not initialized (e.g "new project" otherwise it's a "open project")
        supportedLangagesMap[languages.at(i)]->sub->initialize();

    }
    pd.setValue(i);
    qApp->processEvents();
    pd.close();
}

/******************************************************************************
 * <SLOT>
 * Generated the GUI elements after the project's configuration.
 ******************************************************************************/
void mainwindow::generateGUI()
{
    initializeMDIMode();
    updateStatusBar();
    createLanguagesTreeNodes();
    disableActions(false);
}

/******************************************************************************
 * <SLOT>
 * Closes the current subtitle editor.
 ******************************************************************************/
void mainwindow::closeCurrentEditor()
{
    mdiArea->activeSubWindow()->close();
}

/******************************************************************************
 * <SLOT>
 * Closes ALL subtitle editors.
 ******************************************************************************/
void mainwindow::closeAllEditors()
{
    mdiArea->closeAllSubWindows();
}

/******************************************************************************
 * <SLOT>
 * Tiles the visible windows.
 ******************************************************************************/
void mainwindow::tileWindows()
{
    mdiArea->tileSubWindows();
}

/******************************************************************************
 * <SLOT>
 * Cascades the visible windows.
 ******************************************************************************/
void mainwindow::cascadeWindows()
{
    mdiArea->cascadeSubWindows();
}

/******************************************************************************
 * <SLOT>
 * Shows the project's tree dockable panel.
 ******************************************************************************/
void mainwindow::showProjectPropertiesDock()
{
    addDockWidget(Qt::RightDockWidgetArea, projectPropertiesDockWidget);
    projectPropertiesDockWidget->show();

}

/******************************************************************************
 * <SLOT>
 * Shows the Audio recorder dockable panel.
 ******************************************************************************/
void mainwindow::showDockableAudioRecorder()
{
    audioRecorder->showDockableAudioRecorder();
}

/******************************************************************************
 * <SLOT>
 * Shows the configuration dialog for the generation of the sound files from
 * text, using the TTS system.
 ******************************************************************************/
void mainwindow::showMenuTTSConfig()
{
    langAudioGen->setSupportedLanguages(supportedProjectLanguages);
    langAudioGen->show();
}


/******************************************************************************
 * <SLOT>
 * Starts the generation of the sound files from text invoking the TTS system.
 ******************************************************************************/
void mainwindow::startTTSAudioGeneration(QStringList &langsToGenerate)
{
    //Progress Dialog initalization
    progressLabel = new QLabel();
    progressLabel->setText(QString("Text-to-speech Audio Generation"));
    progressLabel->setAlignment(Qt::AlignCenter);


    progressDialog = new QProgressDialog(this);
    progressDialog->setLabel(progressLabel);
    progressDialog->setModal(true);
    progressDialog->setAutoReset(false);
    progressDialog->setAutoClose(false);
    progressDialog->setCancelButton(0);
    progressDialog->setFixedSize(350,100);
    progressDialog->setMinimum(0);
    //Num of langs * num of rows * (Subs + AD)
    progressDialog->setMaximum(langsToGenerate.count()*NUMBEROFROWS*2);
    progressDialog->show();

    //Festival Generator Setup
    FestivalAudioGenerator *festAudioGen = new FestivalAudioGenerator();
    //    QString voice = QString("(voice_rab_diphone)");
    //    festAudioGen->setVoiceName(voice);

    QString *dirPath = new QString;
    QString *subText = new QString;
    QString *fileName = new QString;
    QString *fullPath = new QString;
    QString *type = new QString("riff");
    QString *subAudioFilePrefix = new QString(PREFIX_SUB_AUDIO_FILE);
    QString *adAudioFilePrefix = new QString(PREFIX_AD_AUDIO_FILE);

    for(int i=0; i < langsToGenerate.count(); i++)
    {
        dirPath->clear();
        dirPath->append(projectBasePath);
        dirPath->append(QString("/"));
        dirPath->append(QString(langsToGenerate.at(i)));
        dirPath->append(QString("/"+AUDIO_FOLDER_NAME+"/"));

        Subtitles *subtitle = supportedLangagesMap[langsToGenerate.at(i)]->sub;

        QString voice = supportedLangagesMap[langsToGenerate.at(i)]->voiceName;
        festAudioGen->setVoiceName(voice);

        subtitle->writeSoundFilesFromText(festAudioGen,progressDialog,
                                          type,dirPath,subAudioFilePrefix,5);
        subtitle->writeSoundFilesFromText(festAudioGen,progressDialog,
                                          type,dirPath,adAudioFilePrefix,6);


    }
    progressDialog->close();

    delete(progressDialog);
    delete(subAudioFilePrefix);
    delete(adAudioFilePrefix);
    delete(dirPath);
    delete(fullPath);
    delete(fileName);
    delete(subText);
    delete(type);
}

/******************************************************************************
 * <SLOT>
 * Loads a STR file to the current active
 ******************************************************************************/
void mainwindow::loadSrtFileToActiveSubtitle()
{
    QMdiSubWindow *subWindow = mdiArea->currentSubWindow();
    if(subWindow==0){
        return;
    }

    Subtitles *subTableWidget = (Subtitles*)(subWindow->widget());

    QString fileName = QFileDialog::getOpenFileName(
            this,
            tr("Open SRT File"),
            QDir::currentPath(),
            tr("STR file (*.srt)") );

    if(fileName!=0 && !fileName.isEmpty())
    {
        subTableWidget->loadSRTArchive(fileName);
    }

}

/******************************************************************************
 * <SLOT>
 * Shows QT information
 ******************************************************************************/
void mainwindow::showQTFrameworkInfo()
{
    QApplication::aboutQt();
}

/******************************************************************************
 * <SLOT>
 * Shows About information
 ******************************************************************************/
void mainwindow::showAboutInfo()
{
    aboutDialog->show();
}


void mainwindow::createLanguagesTreeNodes()
{
    QList<QString> langs = supportedLangagesMap.keys();
    for(int i=0; i < langs.size(); i++){

        if(supportedLangagesMap[langs.at(i)]->isSupported){
            languagesTreeNode->addChild( supportedLangagesMap[langs.at(i)]->treeNodeItem );
        }
    }
}

/******************************************************************************
 * Intializes the multiple display windows mode.
 ******************************************************************************/
void mainwindow::initializeMDIMode()
{
    QList<QString> langs = supportedLangagesMap.keys();

    for(int i=0; i<langs.count(); i++){
        if(supportedLangagesMap[langs.at(i)]->isSupported)
        {
            QMdiSubWindow *subWindow = mdiArea->addSubWindow(supportedLangagesMap[langs.at(i)]->sub);
            subWindow->setWindowTitle(langs.at(i));
            subWindow->setWindowIcon(supportedLangagesMap[langs.at(i)]->icon);

            subWindow->showNormal();

            //save subwindow for later closing/opening events
            supportedLangagesMap[langs.at(i)]->subWindow = subWindow;

            QString text(langs.at(i));
            QAction *action = new QAction(supportedLangagesMap[langs.at(i)]->icon
                                          ,text
                                          ,supportedLangagesMap[langs.at(i)]->sub);
            action->setCheckable(true);
            menuWindows->addAction(action);

            connect(action, SIGNAL(triggered()), subWindow, SLOT(show()));
            connect(action, SIGNAL(triggered()), subWindow, SLOT(setFocus()));
        }
    }

    mdiArea->show();
    mdiArea->cascadeSubWindows();
}

/******************************************************************************
 * Creates the project's properties tree.
 ******************************************************************************/
void mainwindow::createProjectTree()
{
    initializeLanguageTree();
    initializeActorsTree();

    projectTree = new QTreeWidget(0);
    projectTree->setColumnCount(1);
    QTreeWidgetItem* rootProjectItem = new QTreeWidgetItem(0);
    rootProjectItem->setText(0,"Properties");
    //rootProjectItem->setText(1,"Play Name");
    projectTree->setHeaderItem(rootProjectItem);;
    projectTree->setEditTriggers(QAbstractItemView::AllEditTriggers);
    projectTree->insertTopLevelItem(0,actorsTreeNode);
    projectTree->insertTopLevelItem(0,languagesTreeNode);
    projectTree->expandAll();
    projectTree->show();

    projectPropertiesDockWidget = new QDockWidget(tr("Details"));
    projectPropertiesDockWidget->setObjectName("projectTreeDockWidget");
    projectPropertiesDockWidget->setWidget(projectTree);
    projectPropertiesDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea
                                                 | Qt::RightDockWidgetArea
                                                 | Qt::BottomDockWidgetArea
                                                 | Qt::TopDockWidgetArea);
    projectPropertiesDockWidget->setToolTip(tr("Project Details"));
    projectPropertiesDockWidget->setMinimumWidth(150);
    projectPropertiesDockWidget->setMaximumWidth(250);

}

/******************************************************************************
 * Intializes the languages tree with the basic elements.
 ******************************************************************************/
void mainwindow::initializeLanguageTree(){
    languagesTreeNode = new QTreeWidgetItem(0);
    languagesTreeNode->setText(0,"Languages");
    languagesTreeNode->setFlags(Qt::ItemIsEnabled);
    languagesTreeNode->setIcon(0,QIcon(":/icons/langsIcon.png"));
    languagesTreeNode->setBackgroundColor(0, QColor("yellow"));


    //receive notifs from the Project Config Dialog
//    connect(projectConfigDialog,SIGNAL(setSupportedLanguage(int)), this, SLOT(setSupportedLanguage(int)));
//    connect(projectConfigDialog,SIGNAL(generateGUI()), this, SLOT(generateGUI()));
}

/******************************************************************************
 * Intializes the supported languages map of the project.
 * This mapping is central point in the whole project, here are actually stored
 * the projects languages and contents for each window.
 * Changes in this function must be wisely done, because they have a wide system
 * impact.
 ******************************************************************************/
void mainwindow::initializeSupportedLanguagesMap(){
    //init SubData structs
    spanishData = new SubData();
    englishData = new SubData();
    catalanData = new SubData();
    frenchData  = new SubData();
    portugueseData = new SubData();

    //create subtitles (must call initialize() before using)
    englishSubtitles = new Subtitles(0);
    spanishSubtitles = new Subtitles(0);
    catalanSubtitles = new Subtitles(0);
    italianSubtitles = new Subtitles(0);

    //initially none is included in the project
    supportedLangagesMap[ENGLISH] = englishData;
    supportedLangagesMap[ENGLISH]->treeNodeItem = english;
    supportedLangagesMap[ENGLISH]->sub = englishSubtitles;
    supportedLangagesMap[ENGLISH]->isSupported = false;
    supportedLangagesMap[ENGLISH]->icon = QIcon(":/icons/flag_gb.png");
    supportedLangagesMap[ENGLISH]->voiceName = QString("(voice_ked_diphone)");

    supportedLangagesMap[SPANISH] = spanishData;
    supportedLangagesMap[SPANISH]->treeNodeItem = spanish;
    supportedLangagesMap[SPANISH]->sub = spanishSubtitles;
    supportedLangagesMap[SPANISH]->isSupported = false;
    supportedLangagesMap[SPANISH]->icon = QIcon(":/icons/flag_spain.png");
    supportedLangagesMap[SPANISH]->voiceName = QString("(voice_el_diphone)");

    supportedLangagesMap[ITALIAN] = portugueseData;
    supportedLangagesMap[ITALIAN]->treeNodeItem = italian;
    supportedLangagesMap[ITALIAN]->sub = italianSubtitles;
    supportedLangagesMap[ITALIAN]->isSupported = false;
    supportedLangagesMap[ITALIAN]->icon = QIcon(":/icons/flag_portugal.png");
    supportedLangagesMap[ITALIAN]->voiceName = QString("(voice_pc_diphone)");

    supportedLangagesMap[CATALAN] = catalanData;
    supportedLangagesMap[CATALAN]->treeNodeItem = catala;
    supportedLangagesMap[CATALAN]->sub = catalanSubtitles;
    supportedLangagesMap[CATALAN]->isSupported = false;
    supportedLangagesMap[CATALAN]->icon = QIcon(":/icons/flag_catalonia.png");
    supportedLangagesMap[CATALAN]->voiceName = QString("(voice_upc_ca_pol_hts)");

    supportedLangagesMap[FRENCH] = frenchData;
    supportedLangagesMap[FRENCH]->treeNodeItem = french;
    supportedLangagesMap[FRENCH]->sub = frenchSubtitles;
    supportedLangagesMap[FRENCH]->isSupported = false;
    supportedLangagesMap[FRENCH]->icon = QIcon(":/icons/flag_france.png");
    supportedLangagesMap[FRENCH]->voiceName = QString("(voice_upc_ca_pol_hts)");

    //initialize all language tree nodes in bulk
    QList<QString> langs = supportedLangagesMap.keys();
    for(int i=0; i < langs.size(); i++){

        QTreeWidgetItem *treeItem = new QTreeWidgetItem(0);
        treeItem = new QTreeWidgetItem(0);
        treeItem->setText(0, langs.at(i) );
        treeItem->setDisabled(false);
        treeItem->setIcon(0, supportedLangagesMap[langs.at(i)]->icon );
        treeItem->setBackgroundColor(0, QColor("yellow"));

        supportedLangagesMap[langs.at(i)]->treeNodeItem = treeItem;
    }
}

/******************************************************************************
 * Intializes the actor's tree, basic structure.
 ******************************************************************************/
void mainwindow::initializeActorsTree()
{
    actorsTreeNode = new QTreeWidgetItem(0);
    actorsTreeNode->setText(0,"Actors");
    actorsTreeNode->setFlags(Qt::ItemIsEnabled);
    actorsTreeNode->setIcon(0,QIcon(":/icons/actorIcon.png"));
    actorsTreeNode->setBackgroundColor(0,QColor("cyan"));
}


/******************************************************************************
 * Gets the subtitles structure from the active languages table editor.
 * Returns:
 * - Subtitles* - the Subtutle tablewidget structure.
 ******************************************************************************/
Subtitles* mainwindow::activeLanguageEditor()
{
    QMdiSubWindow *subWindow = mdiArea->activeSubWindow();
    if (subWindow)
        return qobject_cast<Subtitles *>(subWindow->widget());
    return 0;
}






