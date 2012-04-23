#include "subtitlelinewidget.h"

SubtitleLineWidget::SubtitleLineWidget(QWidget *parent) :
    QWidget(parent)
{
    createElements();
    setLayout(mainGridLayout);
    show();

}

void SubtitleLineWidget::createElements()
{
    mainGridLayout = new QGridLayout;
    initTime    = new QLabel(QString("Initial Time: "));
    endTime     = new QLabel(QString("End Time: "));
    subText     = new QLabel(QString("Subtitle: "));
    adText      = new QLabel(QString("AD: "));
    subAudioFile= new QLabel(QString("Subtile Sound: "));;
    adAudioFile = new QLabel(QString("AD Sound: "));

    subTextEdit = new QTextEdit;


    mainGridLayout->addWidget(initTime,0,0,Qt::AlignLeft);

    mainGridLayout->addWidget(endTime,1,0,Qt::AlignLeft);

    mainGridLayout->addWidget(subText,0,2,Qt::AlignLeft);
    mainGridLayout->addWidget(subTextEdit,0,3,Qt::AlignLeft);

    mainGridLayout->addWidget(adText,1,2,Qt::AlignLeft);

    mainGridLayout->addWidget(subAudioFile,0,4,Qt::AlignLeft);

    mainGridLayout->addWidget(adAudioFile,1,4,Qt::AlignLeft);

}


