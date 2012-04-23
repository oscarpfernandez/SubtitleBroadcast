#ifndef LANGUAGEDIALOG_H
#define LANGUAGEDIALOG_H

#include <QListWidget>
#include <QDialog>


class LanguageDialog : public QDialog
{
    Q_OBJECT
public:
    LanguageDialog();

private:
    QListWidget *leftLanguageList;
    QListWidget *rightLanguageList;

public slots:
    void moveLeft();
    void moveRight();

};

#endif // LANGUAGEDIALOG_H
