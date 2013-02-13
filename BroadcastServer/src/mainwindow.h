/******************************************************************************
 * STR2 Content Broadcast Server
 *
 * Developed by Oscar Lopes as the final project of the Multimedia Technologies
 * Master, and sponsored by CAIAC (caiac.uab.cat)
 *
 * (C)2011 [Universidad Autònoma de Barcelona]
 ******************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Qt/qapplication.h>
#include <QFileDialog>
#include <QString>
#include <QFileInfo>
#include <QAction>
#include <QToolBar>
#include "xmlprojectreader.h"
#include "subtitleslaunchdialog.h"
#include "projectproperties.h"
#include "utils.h"

class mainwindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit mainwindow(QWidget *parent = 0);

private:
    QAction *loadProjectAction;
    QAction *exitAppAction;
    QAction *helpAboutQTAction;
    QAction *helpAboutAction;
    QAction *manualModeAction;
    QAction *automaticModeAction;
    QAction *startSessionAction;
    QAction *stopSessionAction;
    QMenu   *fileMenu;
    QMenu   *modeMenu;
    QMenu   *helpMenu;
    QToolBar *mainToolbar;

    SubtitlesLaunchDialog *subLaunchDiag;
    ProjectProperties     *projectPropsDialog;
    //name,description,path
    QMap<QString,QString> projectPropertiesMap;
    QStringList availableLanguages;
    QString displayLanguage;
    QString projectPath;
    QString unixUsername;



    bool isSubtitleDataLoaded;

    void createActions();
    void createMenus();
    void createMainToolbar();
    void loadDataFromXML();

public slots:
    void setLangAndLoadData(QString &displayLanguage);

private slots:
    void loadProject();
    void closeApplication();
    void showAboutQT();
    void showAbout();
    void setManualMode();
    void setAutomaticMode();
    void enableModeMenu();
    void startBroadcastAction();
    void stopBroadcastAction();

};

#endif // MAINWINDOW_H
