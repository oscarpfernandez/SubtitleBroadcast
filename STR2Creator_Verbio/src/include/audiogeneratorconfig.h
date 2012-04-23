#ifndef AUDIOGENERATORCONFIG_H
#define AUDIOGENERATORCONFIG_H

#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QTextEdit>
#include <QProgressDialog>
#include <QListWidget>
#include "src\include\utils.h"
#include "src\include\constansts.h"

class AudioGeneratorConfig : public QDialog
{
    Q_OBJECT
public:
    explicit AudioGeneratorConfig(QWidget *parent = 0);

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
    QStringList langsToGenerate;

    QProgressDialog *progressDialog;
    QLabel *progressLabel;

    void createGuiElements();
    void setSupportedLanguages(QStringList &langsSupported);

private slots:
    void moveLanguageLeft();
    void moveLanguageRight();
    void okStartAudioGeneration();

signals:
    void startTTSAudioGeneration(QStringList &languages);
};

#endif // AUDIOGENERATORCONFIG_H
