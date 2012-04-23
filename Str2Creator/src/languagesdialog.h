#ifndef LANGUAGESDIALOG_H
#define LANGUAGESDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QGridLayout>
#include <QListWidget>

class LanguagesDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LanguagesDialog(QWidget *parent = 0);
    void showLanguagesDialog();

private:
    QDialog *languageDialog;
    QPushButton *leftButton, *rightButton;
    QGridLayout *layout;
    QStringList languageItems;
    QStringList items;
    QListWidget *leftLanguageList;
    QListWidget *rightLanguageList;


private slots:
    void moveLanguageLeft();
    void moveLanguageRight();

};

#endif // LANGUAGESDIALOG_H
