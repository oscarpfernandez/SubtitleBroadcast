/******************************************************************************
 * STR2 Content Broadcast Server
 *
 * Developed by Oscar Lopes as the final project of the Multimedia Technologies
 * Master, and sponsored by CAIAC (caiac.uab.cat)
 *
 * (C)2011 [Universidad Autònoma de Barcelona]
 ******************************************************************************/

#ifndef SUBTITLESLAUNCHDIALOG_H
#define SUBTITLESLAUNCHDIALOG_H

#include <QDialog>
#include <QFont>
#include <QListWidget>
#include <QLabel>
#include <QStatusBar>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QKeyEvent>
#include "constants.h"
#include "xmlprojectreader.h"
#include "projectproperties.h"
#include "subtitledata.h"
#include "udpcontentserver.h"
#include "udplanguagesserver.h"

class SubtitlesLaunchDialog : public QWidget
{
    Q_OBJECT

public:
    SubtitlesLaunchDialog(QWidget *parent = 0);
    void setVisibleSubtitleTable(QString &lang);
    void createSubtitleTable();
    void loadSubtitleDataFromXML(QString &projectPath,
                                 QStringList &languageList);
    void setManualMode(bool mode);
    void startBroadcastAction();
    void stopBroadcastAction();
    void loadTimestampsLaunchTicks();
    ~SubtitlesLaunchDialog();

protected:
    void keyPressEvent(QKeyEvent *event);

private:

    int currentSubLineIndex;                //the current subtitle line waiting to be sent...
    int rowTimeStampInMilis[NUM_OF_ROWS];   //Has the timeStamps of the several entries converted to milis
    int rowTimeStampEndInMilis[NUM_OF_ROWS];
    QFont item_font;                        //Text font
    QTableWidget *subtileTableWidget;       //Display table
    QLabel *lTimer;
    QLabel *totalLines;
    QLabel *label_try;
    QLabel *autoMsm;
    QLabel *manualMsm;
    QStatusBar *stat_bar;
    QTime clockTime;
    QTimer *timerCount;
    QTimer *sendInitSubsTimer;
    QTimer *sendEndSubsTimer;
    int totalTimeCount;
    //bool isBroadCastPaused;

    QPushButton *sButton;                   //Start button
    QPushButton *pButton;                   //Pause Button
    QPushButton *qButton;                   //Stop button
    QPushButton *sendSubsButton;
    QPushButton *sendShake;
    QPushButton *startButton;
    QPushButton *stopButton;


    QVBoxLayout *mainlayout;
    QHBoxLayout *horizlayout;
    QVBoxLayout *verticallayout;
    QGridLayout *aLayout;
    QGroupBox   *agridGroupBox;
    QGroupBox   *mgridGroupBox;
    QGroupBox   *langSelectionGroupBox;
    QGridLayout *mLayout;
    QLabel      *langsLabel;
    QHBoxLayout *langHLayout;
    QComboBox   *langsComboBox;

    XMLProjectReader *xmlReader;
    QStringList availableLanguagesList;
    QString displayLanguage;
    SubtitleData *subtitlesData;            //Data container
    UDPContentServer *udpServerEnglish;     //UDP Trasmission Socket
    UDPContentServer *udpServerCatalan;     //UDP Trasmission Socket
    UDPContentServer *udpServerSpanish;     //UDP Trasmission Socket
    UDPContentServer *udpServerItalian;     //UDP Trasmission Socket
    UDPLanguagesServer *udpLanguageServer;  //UDP Trasmission Socket
    int currentTableLine;
    bool isSubtitleProjectLoaded;
    bool isAutoModeEnabled;
    bool isSessionPaused;
    bool isLineIndexChanged;
    void createElements();
    void createTimers();
    void loadDataToTableWidget(QMap<int,SubtitleLineData*> *subtitleDat);
    void initializeSubtitleData();
    void setComboBoxToText(QComboBox *combo, QString &text);
    void enableElements(bool enable);
    void broadCastContents(int index);
    void broadCastHapticSignal();
    bool checkTimeStampFormat(QString &timeStamp);
    QString createFormattedTimeStamp(int timeMilis);
    int convertTimeStampToMilis(QString timeStampp);
    int calculateLaunchTimeInterval(int baseIndex);
    int calculateRemoveTimeInterval(int baseIndex);
    void resetTimerCounter();
    void resetTimerCounterForNewStart(int index);
    void setRunningTimer(int timeInMilis);

private slots:
    void languageComboBoxSelectionChanged(QString lang);
    void sendSubsButtonAction();
    void updateRunningTimer();
    void startAutoTimer();
    void pauseAutoTimer();
    void stopAutoTimer();
    void sendSubsTimerTick();
    void sendEmptySubsTimerTick();

signals:
    void enableMainWindowMenus();
    void sessionResumed();
    void broadcastedCurrentContent(QString& text);

};

#endif // SUBTITLESLAUNCHDIALOG_H
