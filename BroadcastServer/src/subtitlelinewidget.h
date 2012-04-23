#ifndef SUBTITLELINEWIDGET_H
#define SUBTITLELINEWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QTextEdit>

class SubtitleLineWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SubtitleLineWidget(QWidget *parent = 0);

private:
    QGridLayout *mainGridLayout;

    QLabel *initTime;
    QLabel *endTime;
    QLabel *subText;
    QLabel *adText;
    QLabel *subAudioFile;
    QLabel *adAudioFile;

    QTextEdit *subTextEdit;

    void createElements();
};

#endif // SUBTITLELINEWIDGET_H
