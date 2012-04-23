#ifndef SUBTITLELINEVIEWER_H
#define SUBTITLELINEVIEWER_H

#include <QDialog>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QSpinBox>
#include <QDockWidget>
#include <QMainWindow>
#include "constansts.h"

class SubtitleLineViewer : public QDialog
{
    Q_OBJECT
public:
    explicit SubtitleLineViewer(QWidget *parent = 0, QMainWindow *window=0);
    void activateDockableSubtitleLineViewer();


private:
    QMainWindow *mainwin;
    QHBoxLayout *mainSubtitleViewHLayout;
    QVBoxLayout *controlsViewVLayout;

    QGroupBox   *controlsGroupBox;
    QGroupBox   *subtitlesGroupBox;

    QVBoxLayout *subFinalVLayout;
    QLabel      *subtitleFinalLabel;
    QTextEdit   *subtitleFinalEdit;

    QHBoxLayout *subTextHLayout;
    QLabel      *subTextLabel;
    QLineEdit   *subtitleTextEdit;

    QHBoxLayout *subStartTimeHLayout;
    QLabel      *subStartTimeLabel;
    QLineEdit   *subtitleStartTimeEdit;

    QHBoxLayout *subStopTimeHLayout;
    QLabel      *subStopTimeLabel;
    QLineEdit   *subtitleStopTimeEdit;

    QHBoxLayout *subIndexHLayout;
    QLabel      *subIndexLabel;
    QSpinBox    *subtitleIndexEdit;

    QLabel      *subAudioDescLabel;
    QLineEdit   *subtitleAudiodescriptionEdit;

    QVBoxLayout *subOrigTextVLayout;
    QLabel      *subOrigTextLabel;
    QTextEdit   *subtitleOriginalTextEdit;

    QHBoxLayout *subAudioFileHLayout;
    QLabel      *subAudioFileNameLabel;
    QLineEdit   *subtitleAudioFileNameEdit;

    void createGuiElements();



};

#endif // SUBTITLELINEVIEWER_H
