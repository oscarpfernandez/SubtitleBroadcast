#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QDate>
#include <QTime>
#include <QPushButton>
#include "constansts.h"

class About : public QDialog
{
    Q_OBJECT
public:
    explicit About(QWidget *parent = 0);

private:
    void createElements();
    QVBoxLayout *mainVBoxLayout;
    QHBoxLayout *buttonHBoxLayout;
    QLabel      *imageLabel;
    QTextEdit   *textEdit;

private slots:
    void closeDialog();
};

#endif // ABOUT_H
