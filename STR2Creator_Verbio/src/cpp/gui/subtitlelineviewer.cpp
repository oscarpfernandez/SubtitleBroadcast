#include "src\include\subtitlelineviewer.h"

SubtitleLineViewer::SubtitleLineViewer(QWidget *parent, QMainWindow *window) : QDialog(parent)
{
    mainwin = window;
    createGuiElements();
}

void SubtitleLineViewer::createGuiElements() {
    setWindowTitle("Subtitle PreViewer");

    mainSubtitleViewHLayout = new QHBoxLayout();

    controlsGroupBox = new QGroupBox();
    subtitlesGroupBox = new QGroupBox();

    controlsViewVLayout = new QVBoxLayout();
    subIndexHLayout     = new QHBoxLayout();
    subStartTimeHLayout = new QHBoxLayout();
    subStopTimeHLayout  = new QHBoxLayout();
    subOrigTextVLayout  = new QVBoxLayout();
    subAudioFileHLayout = new QHBoxLayout();
    subFinalVLayout     = new QVBoxLayout();

    subIndexLabel = new QLabel(tr("Index:"));
    subtitleIndexEdit = new QSpinBox;
    subtitleIndexEdit->setMinimum(1);
    subtitleIndexEdit->setMaximum(5000);
    subtitleIndexEdit->setFont(COMMON_LABEL_FONT);
    subIndexHLayout->addWidget(subIndexLabel);
    subIndexHLayout->addWidget(subtitleIndexEdit);


    subStartTimeLabel = new QLabel(tr("Start Time:"));
    subStartTimeLabel->setFont(COMMON_LABEL_FONT);
    subtitleStartTimeEdit = new QLineEdit();
    subtitleStartTimeEdit->setFont(COMMON_LABEL_FONT);
    subtitleStartTimeEdit->setMinimumWidth(65);
    subStartTimeHLayout->addWidget(subStartTimeLabel);
    subStartTimeHLayout->addWidget(subtitleStartTimeEdit);

    subStopTimeLabel = new QLabel(tr("Stop Time:"));
    subStopTimeLabel->setFont(COMMON_LABEL_FONT);
    subtitleStopTimeEdit = new QLineEdit();
    subtitleStopTimeEdit->setFont(COMMON_LABEL_FONT);
    subtitleStopTimeEdit->setMinimumWidth(65);
    subStopTimeHLayout->addWidget(subStopTimeLabel);
    subStopTimeHLayout->addWidget(subtitleStopTimeEdit);

    subOrigTextLabel = new QLabel(tr("Original Text:"));
    subOrigTextLabel->setFont(COMMON_LABEL_FONT);
    subtitleOriginalTextEdit = new QTextEdit();
    subtitleOriginalTextEdit->setFont(COMMON_LABEL_FONT);
    subtitleOriginalTextEdit->setMinimumHeight(40);
    subOrigTextVLayout->addWidget(subOrigTextLabel);
    subOrigTextVLayout->addWidget(subtitleOriginalTextEdit);

    subAudioFileNameLabel = new QLabel(tr("Audio File:"));
    subAudioFileNameLabel->setFont(COMMON_LABEL_FONT);
    subtitleAudioFileNameEdit = new QLineEdit();
    subtitleAudioFileNameEdit->setFont(COMMON_LABEL_FONT);
    subtitleAudioFileNameEdit->setEnabled(false);
    subAudioFileHLayout->addWidget(subAudioFileNameLabel);
    subAudioFileHLayout->addWidget(subtitleAudioFileNameEdit);

    controlsViewVLayout->addLayout(subIndexHLayout);
    controlsViewVLayout->addLayout(subStartTimeHLayout);
    controlsViewVLayout->addLayout(subStopTimeHLayout);
    controlsViewVLayout->addLayout(subAudioFileHLayout);

    subtitleFinalLabel = new QLabel(tr("Translated Subtitle Text Pre-View:"));
    subtitleFinalLabel->setFont(COMMON_LABEL_FONT);
    subtitleFinalEdit = new QTextEdit();
    subtitleFinalEdit->setFont(COMMON_LABEL_FONT);
    subtitleFinalEdit->setMinimumWidth(subtitleFinalEdit->sizeHint().width());
    subtitleFinalEdit->setFont(QFont("Arial", 15, QFont::Bold));
    subtitleFinalEdit->setTextColor(QColor(Qt::darkBlue));
    subtitleFinalEdit->setMinimumHeight(90);
    subtitleFinalEdit->setAlignment(Qt::AlignCenter);
    subFinalVLayout->addLayout(subOrigTextVLayout);
    subFinalVLayout->addWidget(subtitleFinalLabel);
    subFinalVLayout->addWidget(subtitleFinalEdit);

    subAudioDescLabel = new QLabel(tr("Audio description:"));
    subAudioDescLabel->setFont(COMMON_LABEL_FONT);
    subtitleAudiodescriptionEdit = new QLineEdit();
    subtitleAudiodescriptionEdit->setFont(COMMON_LABEL_FONT);
    subFinalVLayout->addWidget(subAudioDescLabel);
    subFinalVLayout->addWidget(subtitleAudiodescriptionEdit);

    controlsGroupBox->setLayout(controlsViewVLayout);
    controlsGroupBox->setMaximumWidth(150);
    controlsGroupBox->setTitle(tr("Configuration"));
    controlsGroupBox->setMaximumHeight(controlsGroupBox->sizeHint().height());
    controlsGroupBox->setAlignment(Qt::AlignTop);
    controlsGroupBox->setFont(COMMON_LABEL_FONT);

    subtitlesGroupBox->setLayout(subFinalVLayout);
    subtitlesGroupBox->setTitle(tr("Subtitles Definition"));
    subtitlesGroupBox->setFont(COMMON_LABEL_FONT);

    mainSubtitleViewHLayout->addWidget(controlsGroupBox);
    mainSubtitleViewHLayout->addWidget(subtitlesGroupBox);;


    setLayout(mainSubtitleViewHLayout);
}

void SubtitleLineViewer::activateDockableSubtitleLineViewer()
{
    QDockWidget *subtitleLineDockWidget = new QDockWidget(tr("Subtitle Pre-Viewer"));
    subtitleLineDockWidget->setObjectName("subTitleDockWidget");
    subtitleLineDockWidget->setWidget(this);
    subtitleLineDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea
                                            | Qt::TopDockWidgetArea);
    mainwin->addDockWidget(Qt::BottomDockWidgetArea, subtitleLineDockWidget);

}


