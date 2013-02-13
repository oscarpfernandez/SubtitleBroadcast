/******************************************************************************
 * STR2 Content Broadcast Server
 *
 * Developed by Oscar Lopes as the final project of the Multimedia Technologies
 * Master, and sponsored by CAIAC (caiac.uab.cat)
 *
 * (C)2011 [Universidad Autònoma de Barcelona]
 ******************************************************************************/

#include "mainwindow.h"

mainwindow::mainwindow(QWidget *parent) :
    QMainWindow(parent)
{
    qDebug("Starting ..."+APP_NAME.toAscii());


    //get unix username
    unixUsername.append(Utils::getCurrentUnixUserName());

    QString title = QString(APP_NAME).append(" ["+unixUsername+"@"+Utils::obtainMachinesIpAddress().toAscii()+"]");

    setWindowTitle(title);
    setWindowIcon(QIcon(":/icons/appicon.png"));
    resize(1000,700);

    //Subtitle data is not loaded
    //isSubtitleDataLoaded = false;

    projectPropsDialog = new ProjectProperties(this);
    connect(projectPropsDialog, SIGNAL(setLangAndLoadData(QString&)),
            this, SLOT(setLangAndLoadData(QString&)));

    subLaunchDiag = new SubtitlesLaunchDialog(0);
    connect(subLaunchDiag,SIGNAL(enableMainWindowMenus()),
            this,SLOT(enableModeMenu()));
    connect(subLaunchDiag,SIGNAL(sessionResumed()),
            this,SLOT(startBroadcastAction()));

    createActions();
    createMenus();
    createMainToolbar();

    setCentralWidget(subLaunchDiag);

}

void mainwindow::createActions()
{
    loadProjectAction = new QAction(tr("&Open Project"), this);
    loadProjectAction->setIcon(QIcon(":/icons/open_project.png"));
    loadProjectAction->setShortcut(tr("Ctrl+O"));
    loadProjectAction->setStatusTip(tr("Load STR2 Project file"));
    connect(loadProjectAction, SIGNAL(triggered()), this, SLOT(loadProject()));

    exitAppAction = new QAction(tr("&Exit"), this);
    exitAppAction->setIcon(QIcon(":/icons/close_app.png"));
    exitAppAction->setShortcut(tr("Ctrl+Q"));
    exitAppAction->setStatusTip(tr("Exit the application"));
    connect(exitAppAction, SIGNAL(triggered()), this, SLOT(closeApplication()));

    helpAboutQTAction = new QAction(tr("About Q&T"), this);
    //helpAboutQTAction->setIcon(QIcon(":/img/about.png"));
    helpAboutQTAction->setShortcut(tr("Ctrl+T"));
    helpAboutQTAction->setStatusTip(tr("About the QT Framework"));
    connect(helpAboutQTAction, SIGNAL(triggered()), this, SLOT(showAboutQT()));

    helpAboutAction = new QAction(tr("A&bout"), this);
    //helpAboutQTAction->setIcon(QIcon(":/img/about.png"));
    helpAboutAction->setShortcut(tr("Ctrl+B"));
    helpAboutAction->setStatusTip(tr("About the application"));
    connect(helpAboutAction, SIGNAL(triggered()), this, SLOT(showAbout()));

    manualModeAction = new QAction(tr("&Manual Mode"),this);
    manualModeAction->setShortcut(tr("Ctrl+M"));
    manualModeAction->setStatusTip(tr("Activate manual mode"));
    manualModeAction->setCheckable(true);
    manualModeAction->setChecked(true);
    manualModeAction->setEnabled(false);
    connect(manualModeAction, SIGNAL(triggered()),this,SLOT(setManualMode()));

    automaticModeAction = new QAction(tr("&Automatic Mode"),this);
    automaticModeAction->setShortcut(tr("Ctrl+M"));
    automaticModeAction->setStatusTip(tr("Activate automatic mode"));
    automaticModeAction->setCheckable(true);
    automaticModeAction->setEnabled(false);
    connect(automaticModeAction, SIGNAL(triggered()),this,SLOT(setAutomaticMode()));

    startSessionAction = new QAction(tr("&Start Session"), this);
    startSessionAction->setShortcut(tr("Ctrl+S"));
    startSessionAction->setStatusTip(tr("Start broadcasting session"));
    startSessionAction->setIcon(QIcon(":/icons/start.png"));
    startSessionAction->setEnabled(false);
    connect(startSessionAction,SIGNAL(triggered()), this ,SLOT(startBroadcastAction()));

    stopSessionAction = new QAction(tr("Stop Sessio&n"), this);
    stopSessionAction->setShortcut(tr("Ctrl+N"));
    stopSessionAction->setStatusTip(tr("Start broadcasting session"));
    stopSessionAction->setIcon(QIcon(":/icons/stop.png"));
    stopSessionAction->setEnabled(false);
    connect(stopSessionAction,SIGNAL(triggered()), this ,SLOT(stopBroadcastAction()));
}

void mainwindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(loadProjectAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAppAction);

    modeMenu = menuBar()->addMenu(tr("&Session Control"));
    modeMenu->addAction(startSessionAction);
    modeMenu->addAction(stopSessionAction);
    modeMenu->addSeparator();
    modeMenu->addAction(automaticModeAction);
    modeMenu->addAction(manualModeAction);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(helpAboutAction);
    helpMenu->addAction(helpAboutQTAction);

}

void mainwindow::createMainToolbar()
{
    mainToolbar = addToolBar("MainToolbar");
    mainToolbar->addAction(loadProjectAction);
    mainToolbar->addSeparator();
    mainToolbar->addAction(startSessionAction);
    mainToolbar->addAction(stopSessionAction);
    mainToolbar->addAction(helpAboutAction);
    mainToolbar->addAction(exitAppAction);
}

/******************************************************************************
 * <SLOT>
 * Loading project's configurations and contents.
 ******************************************************************************/
void mainwindow::loadProject()
{
    QString fileName = QFileDialog::getOpenFileName(
            this,
            tr("Open Project"),
            QDir::currentPath(),
            tr("STR2 Project (project.xml)") );

    update();

    if(fileName.count()==0){
        //the user pressed cancel... nothing to do
        return;
    }

    QMap<QString,QString> retrievedProps;

    if(fileName!=0 && !fileName.isEmpty())
    {
        XMLProjectReader *xmlReader = new XMLProjectReader(this);
        retrievedProps = xmlReader->readProjectDefinitionXML(fileName);
    }

    QFileInfo fi(fileName);

    projectPath = fi.canonicalPath();
    availableLanguages = retrievedProps[STR_LANGUAGES].split(":");

    qDebug("* Recovered project base path: "+projectPath.toAscii());

    projectPropertiesMap = QMap<QString,QString>();
    projectPropertiesMap.insert(PROJ_CONFIG_NAME,retrievedProps[STR_PROJECTNAME]);
    projectPropertiesMap.insert(PROJ_CONFIG_DESC,retrievedProps[STR_PROJECTDESC]);
    projectPropertiesMap.insert(PROJ_CONFIG_LANGS,retrievedProps[STR_LANGUAGES]);
    projectPropertiesMap.insert(PROJ_CONFIG_BASE_PATH,projectPath);

    projectPropsDialog->setProjectProperties(retrievedProps[STR_PROJECTNAME],
                                             retrievedProps[STR_PROJECTDESC],
                                             projectPath,
                                             retrievedProps[STR_LANGUAGES]);


    projectPropsDialog->show();
}

//SLOT
void mainwindow::setLangAndLoadData(QString &displayLang)
{
    qDebug("ENTRY mainwindow::setLangAndLoadData displayLang = "+displayLang.toAscii());
    displayLanguage = displayLang;

    QMessageBox msgBox(this);
    msgBox.setText("Import XML Data");
    msgBox.setWindowModality(Qt::ApplicationModal);
    msgBox.setInformativeText("Importing XML data to session window.\nPlease wait...");
    msgBox.show();
    update();

    loadDataFromXML();

    msgBox.close();

    enableModeMenu();

    qDebug("EXIT mainwindow::setLangAndLoadData");
}

void mainwindow::loadDataFromXML()
{
    qDebug("ENTRY mainwindow::loadDataFromXML");
    subLaunchDiag->loadSubtitleDataFromXML(projectPath, availableLanguages);

    subLaunchDiag->setVisibleSubtitleTable(displayLanguage);

    subLaunchDiag->loadTimestampsLaunchTicks();

    qDebug("EXIT mainwindow::loadDataFromXML");
}


//SLOT
void mainwindow::showAboutQT()
{
    qApp->aboutQt();
}

//SLOT
void mainwindow::showAbout()
{
  QMessageBox::about(this, tr("About"), tr("<h2>"+APP_NAME.toAscii()+" v1.0</h2>"
                                                     "<p>Copyright &copy; UAB 2011"
                                                     "<p>This application delivers in realtime "
                                                     "STR2 content to mobile devices of "
                                                     "spectators in theaters houses"));
}

//Slot
void mainwindow::setAutomaticMode(){

    subLaunchDiag->setManualMode(false);

    automaticModeAction->setChecked(true);
    manualModeAction->setChecked(false);

}

//Slot
void mainwindow::setManualMode()
{
    subLaunchDiag->setManualMode(true);

    automaticModeAction->setChecked(false);
    manualModeAction->setChecked(true);

}

//Slot
void mainwindow::enableModeMenu(){
   startSessionAction->setEnabled(true);
}

//Slot
void mainwindow::startBroadcastAction()
{
    subLaunchDiag->startBroadcastAction();
    manualModeAction->setEnabled(true);
    automaticModeAction->setEnabled(true);
    stopSessionAction->setEnabled(true);
    startSessionAction->setEnabled(false);
}

//Slot
void mainwindow::stopBroadcastAction()
{
    subLaunchDiag->stopBroadcastAction();
    manualModeAction->setEnabled(false);
    automaticModeAction->setEnabled(false);
    startSessionAction->setEnabled(true);
    stopSessionAction->setEnabled(false);
}

/******************************************************************************
 * <SLOT>
 * Close Application.
 ******************************************************************************/
void mainwindow::closeApplication()
{
    qApp->exit(0);
}
