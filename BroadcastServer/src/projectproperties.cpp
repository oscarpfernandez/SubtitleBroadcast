/******************************************************************************
 * STR2 Content Broadcast Server
 *
 * Developed by Oscar Lopes as the final project of the Multimedia Technologies
 * Master, and sponsored by CAIAC (caiac.uab.cat)
 *
 * (C)2011 [Universidad Autònoma de Barcelona]
 ******************************************************************************/

#include "projectproperties.h"

/******************************************************************************
 * Description: the purpose of this class is to create a project setup confi-
 * ration GUI, the allows the user to setup primary project conditions:
 * - base path, supported languages, project name, description, etc.
 ******************************************************************************/

ProjectProperties::ProjectProperties(QWidget *parent) :
        QDialog(parent)
{
    setWindowTitle("Project Properties");
    setModal(true);
    setMinimumSize(400,250);
    setMaximumSize(600,500);

    progressLabel = new QLabel();
    progressLabel->setAlignment(Qt::AlignCenter);

    progressDialog = new QProgressDialog(this);
    progressDialog->setLabel(progressLabel);
    progressDialog->setModal(true);
    progressDialog->setAutoReset(false);
    progressDialog->setAutoClose(false);
    progressDialog->setCancelButton(0);
    progressDialog->setFixedSize(350,100);

    createGuiElements();

    setWindowOpacity(0.85);
}

/******************************************************************************
 * Creates projects GUI layout elements.
 ******************************************************************************/
void ProjectProperties::createGuiElements()
{
    mainProjectConfigVLayout = new QVBoxLayout;
    projectPathConfigLineHLayout = new QHBoxLayout;
    languagesConfigVLayout = new QVBoxLayout;

    //Project Name
    projectName = new QLabel(tr("Project Name:"));
    projectNameLine = new QLineEdit(this);
    projectNameHBoxLayout = new QHBoxLayout;
    projectNameHBoxLayout->addWidget(projectName);
    projectNameHBoxLayout->addWidget(projectNameLine);

    //Project Description Text
    projectDescriptionVBoxLayout = new QVBoxLayout;
    projectBasicPropertiesVLayout = new QVBoxLayout;
    projectDescription = new QLabel("Description:");
    projectDescriptionText = new QTextEdit;
    projectDescriptionText->setMinimumHeight(50);
    projectDescriptionVBoxLayout->addWidget(projectDescription);
    projectDescriptionVBoxLayout->addWidget(projectDescriptionText);

    projectBasicPropertiesVLayout->addLayout(projectNameHBoxLayout);
    projectBasicPropertiesVLayout->addSpacing(10);
    projectBasicPropertiesVLayout->addLayout(projectDescriptionVBoxLayout);

    projectBasicPropertiesGroupBox = new QGroupBox(tr("Basic Properties"));
    projectBasicPropertiesGroupBox->setLayout(projectBasicPropertiesVLayout);

    //Project's Path Configuration
    projectPathGroupBox = new QGroupBox(tr("Path"));
    projectPathLabel = new QLabel(tr("Path:"));
    projectPathLine = new QLineEdit;
    projectPathLine->setReadOnly(true);
    projectPathButton = new QPushButton(tr("Choose"));
    projectPathConfigLineHLayout->addWidget(projectPathLabel);
    projectPathConfigLineHLayout->addWidget(projectPathLine);
//    projectPathConfigLineHLayout->addWidget(projectPathButton);
    projectPathGroupBox->setLayout(projectPathConfigLineHLayout);

    buttonsHLayout = new QHBoxLayout;
    okButton = new QPushButton(tr("Ok"),this);
    okButton->setMaximumWidth(okButton->sizeHint().width());
    cancelButton = new QPushButton(tr("Cancel"),this);
    cancelButton->setMaximumWidth(cancelButton->sizeHint().width());
    cancelButton->setFocus();
    buttonsHLayout->addWidget(cancelButton);
    buttonsHLayout->addWidget(okButton);
    buttonsHLayout->setAlignment(Qt::AlignRight);

    connect( okButton, SIGNAL(clicked()), this, SLOT(okActionSaveConfigurations()));
    connect( cancelButton, SIGNAL(clicked()), this, SLOT(cancelAction()));

    //Display Language Config
    projectDisplayLangGroupBox = new QGroupBox("Session Display Language");
    projectDisplayLangLayout = new QHBoxLayout;
    projectDisplayLanguages = new QComboBox();
    QStringList list;
    list << SPANISH << ITALIAN << ENGLISH << CATALAN;
    projectDisplayLanguages->addItems(list);
    projectDisplayLangText = new QLabel("Language");
    projectDisplayLangLayout->addWidget(projectDisplayLangText);
    projectDisplayLangLayout->addWidget(projectDisplayLanguages);
    projectDisplayLangGroupBox->setLayout(projectDisplayLangLayout);


    mainProjectConfigVLayout->addWidget(projectBasicPropertiesGroupBox);
    mainProjectConfigVLayout->addSpacing(10);
    mainProjectConfigVLayout->addWidget(projectPathGroupBox);
    mainProjectConfigVLayout->addSpacing(10);
    mainProjectConfigVLayout->addWidget(projectDisplayLangGroupBox);
    mainProjectConfigVLayout->addLayout(buttonsHLayout);

    setLayout(mainProjectConfigVLayout);
}
void ProjectProperties::setProjectProperties(const QString &projName,
                                             const QString &projDesc,
                                             const QString &projPath,
                                             const QString &projLangs)
{
    qDebug("ENTRY ProjectProperties::setProjectProperties");
    qDebug("Project Name: "+projName.toAscii()+
           "Project Desc: "+projDesc.toAscii()+
           "Project Path: "+projPath.toAscii()+
           "Project Langs: "+projLangs.toAscii());
    projectDisplayLanguages->clear();
    availableLangs = projLangs.split(":");
    qDebug("availableLangs count = "+availableLangs.count());
    projectDisplayLanguages->addItems(availableLangs);
    projectNameLine->setText(projName);
    projectDescriptionText->setText(projDesc);
    projectPathLine->setText(projPath);
    qDebug("EXIT ProjectProperties::setProjectProperties");

}

void ProjectProperties::okActionSaveConfigurations()
{
    qDebug("ENTRY ProjectProperties::okActionSaveConfigurations");
    if(projectDisplayLanguages->count()==0){
        //this should not happen... if so it means the XML project file is corrupt...
        QMessageBox warningMessage;
        warningMessage.setText("The project file is damaged...\nNo languages found!");
        warningMessage.setWindowTitle("Warning");
        warningMessage.setIcon(QMessageBox::Warning);
        warningMessage.exec();
    }
    else
    {
        //configure language to be used as default of the project...
        QString chosenLang = projectDisplayLanguages->currentText();
        this->close();

        QMessageBox msgBox(this);
        msgBox.setText("Import XML Data");
        msgBox.setWindowModality(Qt::ApplicationModal);
        msgBox.setInformativeText("Importing XML language data.\nPlease wait...");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.show();
        msgBox.update();

        update();

        emit setLangAndLoadData(chosenLang);

        msgBox.close();

        qDebug("ProjectProperties::okActionSaveConfigurations - emit setDisplayLanguage:"+chosenLang.toAscii());
    }

    qDebug("EXIT ProjectProperties::okActionSaveConfigurations");
}

void ProjectProperties::cancelAction()
{
    this->close();
}
