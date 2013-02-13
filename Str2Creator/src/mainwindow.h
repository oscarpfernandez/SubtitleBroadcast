#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <stdio.h>
#include <stdlib.h>
#include <QLabel>
#include <QFile>
#include <QtGui>
#include <QtGui/QApplication>
#include <QtGui/QSplashScreen>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTreeWidget>
#include <QSplitter>
#include <QtMultimedia/QAudioInput>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDockWidget>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <stdio.h>
#include "subtitles.h"
#include "constansts.h"
#include "subtitlelineviewer.h"
#include "audioplayer.h"
#include "audiorecorder.h"
#include "projectsetupconfig.h"
#include "sleeper.h"
#include "basetypes.h"
#include "utils.h"
//#include "audiogeneratorconfig.h"
#include "xmlprojectexport.h"
#include "about.h"

class mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    mainwindow();

private slots:
    void newProject();
    void saveProject();
    bool closeProject();
    void openProject();
    void closeCurrentEditor();
    void closeAllEditors();
    void generateGUI();
    void tileWindows();
    void cascadeWindows();
    void showProjectPropertiesDock();
    void showDockableAudioRecorder();
    void exportProject();
    void importProject();
    //void showMenuTTSConfig();
    void loadSrtFileToActiveSubtitle();
    void showQTFrameworkInfo();
    void showAboutInfo();

public slots:
    void setSupportedLanguages(QStringList languages);
    void setProjectProperties(QMap<QString,QString> &props);
    //void startTTSAudioGeneration(QStringList &langsToGenerate);


private:
    void createElements();
    void createMenus();
    void createActions();
    void disableActions(bool value);
    void createMainToolbar();
    void createStatusbar();
    void updateStatusBar();

    void createProjectTree();
    void initializeLanguageTree();
    void initializeActorsTree();

    void initializeSupportedLanguagesMap();

    void initializeSubtitlesTabGroup();
    void createSubtitlesTab();

    void createLanguagesTreeNodes();
    void initializeMDIMode();

    void closeAllDockablePanels();

    void loadXMLDataToSubtitlesTables(QString &basePath,
                                      QStringList &langiages);

    Subtitles* activeLanguageEditor();
    SubtitleLineViewer *subtitleLineViewer;

    QStringList supportedProjectLanguages;

    //AudioGeneratorConfig *langAudioGen;
    QLabel *progressLabel;
    QProgressDialog *progressDialog;

    QAction *newProjectAction;
    QAction *saveProjectAction;
    QAction *openProjectAction;
    QAction *closeProjectAction;
    QAction *loadSrtFileAction;
    QAction *closeAllEditorsAction;
    QAction *closeCurrentEditorAction;
    QAction *cascadeSubWindowsAction;
    QAction *tileSubWindowsAction;
    QAction *showProjectDockPanelAction;
    QAction *showAudioRecorderDockAction;
    QAction *showMediaPlayerDockPanelAction;
    QAction *aboutQTAction;
    QAction *aboutApplicationAction;

    QAction *exportProjectAction;
    QAction *importProjectAction;

    QAction *generateAudioDescFromTTS;

    QLabel  *locationLabel;
    QLabel  *formulaLabel;
    QMenu   *menuArchive;
    QMenu   *menuTools;
    QMenu   *menuWindows;
    QMenu   *menuHelp;
    QToolBar *fileToolbar;
    QToolBar *toolsToolbar;
    QToolBar *windowToolbar;

    QAudioInput *udio;
    QFile outputFile;

    QString workingDirectory;
    QString workingDirectoryNew;

    QString Path_SRT2;
    QString Directory_AUD;
    QString Directory_DOB;

    QMap<QString, SubData*> supportedLangagesMap;

    QListWidget *leftLanguageList;
    QListWidget *rightLanguageList;


    //Language tree data
    QTreeWidget *projectTree;
    QTreeWidgetItem *spanish;
    QTreeWidgetItem *italian;
    QTreeWidgetItem *catala;
    QTreeWidgetItem *english;
    QTreeWidgetItem *french;
    QTreeWidgetItem *actorsTreeNode;
    QTreeWidgetItem *languagesTreeNode;

    //Language Tab data
//    QTabWidget *subtitlesTabsGroup;
//    QTab *spanishSubTab;

    QDockWidget *audioPlayerDockWidget;
    QDockWidget *audioRecorderDockWidget;
    QDockWidget *projectPropertiesDockWidget;

    //Subtitles Tables
    Subtitles *spanishSubtitles;
    Subtitles *englishSubtitles;
    Subtitles *catalanSubtitles;
    Subtitles *frenchSubtitles;
    Subtitles *italianSubtitles;

    //Language Support data structure:
    SubData *spanishData;
    SubData *englishData;
    SubData *catalanData;
    SubData *frenchData;
    SubData *portugueseData;

    AudioPlayer *audioPlayer;
    AudioRecorder *audioRecorder;

    //Initial Configuration Dialog (base path, languages)
    ProjectSetUpConfig *projectConfigDialog;

    //Flag to check if a project is currently opened.
    bool isProjectOpened;

    //Project's name
    QString projectName;
    //Project's base path
    QString projectBasePath;
    //Project Description
    QString projectDescription;

    QMap<QString,QString> projectProperties;

    //Multiple Documents support
    QMdiArea *mdiArea;

    About *aboutDialog;

};


#endif // MAINWINDOW_H
