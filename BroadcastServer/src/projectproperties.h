/******************************************************************************
 * STR2 Content Broadcast Server
 *
 * Developed by Oscar Lopes as the final project of the Multimedia Technologies
 * Master, and sponsored by CAIAC (caiac.uab.cat)
 *
 * (C)2011 [Universidad Autònoma de Barcelona]
 ******************************************************************************/

#ifndef PROJECTPROPERTIES_H
#define PROJECTPROPERTIES_H

#include <QDialog>
#include <QLabel>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QGroupBox>
#include <QProgressDialog>
#include <QPushButton>
#include <QComboBox>
#include <QMessageBox>
#include "constants.h"

class ProjectProperties : public QDialog
{
    Q_OBJECT
public:
    explicit ProjectProperties(QWidget *parent = 0);
    void setProjectProperties(const QString &projName,
                              const QString &projDesc,
                              const QString &projPath,
                              const QString &projLangs);

private:
    QVBoxLayout *mainProjectConfigVLayout;
    QHBoxLayout *projectPathConfigLineHLayout;
    QVBoxLayout *languagesConfigVLayout;

    QVBoxLayout *projectBasicPropertiesVLayout;
    QGroupBox   *projectBasicPropertiesGroupBox;
    QHBoxLayout *projectNameHBoxLayout;
    QLabel      *projectName;
    QLineEdit   *projectNameLine;

    QVBoxLayout *projectDescriptionVBoxLayout;
    QLabel      *projectDescription;
    QTextEdit   *projectDescriptionText;

    QGroupBox   *projectDisplayLangGroupBox;
    QHBoxLayout *projectDisplayLangLayout;
    QLabel      *projectDisplayLangText;
    QComboBox   *projectDisplayLanguages;

    QGroupBox   *projectPathGroupBox;
    QLabel      *projectPathLabel;
    QLineEdit   *projectPathLine;
    QPushButton *projectPathButton;

    QHBoxLayout *buttonsHLayout;
    QPushButton *okButton;
    QPushButton *cancelButton;

    QStringList supportedLangs;
    QString projectBaseFolder;

    QLabel *progressLabel;
    QProgressDialog *progressDialog;

    QStringList availableLangs;

    void createGuiElements();
    void changeSupportedLanguages();

private slots:
    void cancelAction();
    void okActionSaveConfigurations();

signals:
    void setLangAndLoadData(QString &displayLanguage);

};

#endif // PROJECTPROPERTIES_H
