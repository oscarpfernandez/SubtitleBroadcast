#ifndef MONITORWID_H
#define MONITORWID_H

#include <QAction>
#include <QDialog>
#include <QFileDialog>
#include <QFont>
#include <QLabel>
#include <QListWidget>
#include <QMenuBar>
#include <QWidget>
#include <QStatusBar>
#include <QFrame>
#include <QFont>
#include <string>
#include "app_fcts.h"

using namespace std;



class Displist : public QDialog
{
    Q_OBJECT

public:
    Displist(QWidget *parent = 0);
    ~Displist();
    int getFailInit();

protected:
    void keyPressEvent(QKeyEvent *event);
    void scanLinetags(string line, string* subText,string* TeSt,int* timeStamp);
    void loadTstamp();
    int load_sub_file();
private slots:
    void load_dialog();
    void about_dialog();
    void autoSendtext();
    void startauto();
    void pauseauto();
    void stopauto();
    void runningTime();

private:
    unsigned int hour,minute,second,mili;
    char* lines[MAX_FILE_LINES]; // array where the file contents is loaded
    char teStamps[MAX_FILE_LINES][25]; // array where the file contents is loaded
    int tStamps[MAX_FILE_LINES];
    char filename[MAX_FILE_NAME]; // name of file to be loaded
    int sockfd; // UDP socket descriptor
    int init_flag; // initialization status: 0 = ok, 1 = socket problem, 2 = file problem
    int crt_line; // index of current line to be sent
    int no_lines; // number of lines in the array
    char autosend_status;
    char Bpause;
    QFont item_font; // text font
    QListWidget *list_wid;
    QLabel *lTimer;
    QLabel *totalLines;
    QStatusBar *stat_bar;
    QMenuBar *menu_bar;
    QMenu *file_menu, *help_menu;
    QAction *load_file, *exit_app, *help_about;
    QTimer *timerCount;
    QTimer *sendShot;
};

#endif
