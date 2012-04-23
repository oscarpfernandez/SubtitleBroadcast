#ifndef PROJECTSETUPCONFIG_H
#define PROJECTSETUPCONFIG_H

#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QTextEdit>
#include <QStringList>
#include <QProgressDialog>
#include "languagesdialog.h"
#include "xmlprojectexport.h"
#include "constansts.h"

class ProjectSetUpConfig : public QDialog
{
    Q_OBJECT
public:
    explicit ProjectSetUpConfig(QWidget *parent = 0);

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

    QGroupBox   *projectPathGroupBox;
    QLabel      *projectPathLabel;
    QLineEdit   *projectPathLine;
    QPushButton *projectPathButton;

    QGroupBox   *languagesGroupBox;
    QGridLayout *languagesLayout;
    QPushButton *leftButton, *rightButton;
    QStringList languageItems;
    QListWidget *leftLanguageList;
    QListWidget *rightLanguageList;

    QHBoxLayout *buttonsHLayout;
    QPushButton *okButton;
    QPushButton *cancelButton;

    QStringList supportedLangs;
    QString projectBaseFolder;

    QLabel *progressLabel;
    QProgressDialog *progressDialog;

    void createGuiElements();
    void changeSupportedLanguages();

private slots:
    void moveLanguageLeft();
    void moveLanguageRight();
    void okActionSaveConfigurations();
    void setUpProjectBaseDir();

signals:
//    void setProjectName(QString &projectName);
//    void setProjectPath(QString &projectPath);
//    void setSupportedLanguage(int value);
    void setProjectProperties(QMap<QString,QString> &props);
    void generateGUI();

};

#endif // PROJECTSETUPCONFIG_H
