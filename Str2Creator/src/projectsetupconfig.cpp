#include "projectsetupconfig.h"
#include "mainwindow.h"

/******************************************************************************
 * Description: the purpose of this class is to create a project setup confi-
 * ration GUI, the allows the user to setup primary project conditions:
 * - base path, supported languages, project name, description, etc.
 ******************************************************************************/

ProjectSetUpConfig::ProjectSetUpConfig(QWidget *parent) :
        QDialog(parent)
{
    setWindowTitle("Project Configuration");
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
void ProjectSetUpConfig::createGuiElements()
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
    projectPathGroupBox = new QGroupBox(tr("Path Configuration"));
    projectPathLabel = new QLabel(tr("Path:"));
    projectPathLine = new QLineEdit;
    projectPathLine->setReadOnly(true);
    projectPathButton = new QPushButton(tr("Choose"));
    projectPathConfigLineHLayout->addWidget(projectPathLabel);
    projectPathConfigLineHLayout->addWidget(projectPathLine);
    projectPathConfigLineHLayout->addWidget(projectPathButton);
    projectPathGroupBox->setLayout(projectPathConfigLineHLayout);

    //Languages configuration
    languagesGroupBox = new QGroupBox(tr("Supported Languages"));
    languagesLayout = new QGridLayout();
    languagesLayout->addWidget( rightButton = new QPushButton( ">>" ), 1, 1 );
    languagesLayout->addWidget( leftButton = new QPushButton( "<<" ), 0, 1 );
    languagesLayout->addWidget( leftLanguageList = new QListWidget, 0, 0, 3, 1 );
    languagesLayout->addWidget( rightLanguageList = new QListWidget, 0, 2, 3, 1 );
    languagesGroupBox->setLayout(languagesLayout);

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
    connect( cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    connect( leftButton, SIGNAL(clicked()), this, SLOT(moveLanguageLeft()) );
    connect( rightButton, SIGNAL(clicked()), this, SLOT(moveLanguageRight()) );
    connect( projectPathButton, SIGNAL(clicked()),this, SLOT(setUpProjectBaseDir()));

    languageItems << SPANISH << ITALIAN << ENGLISH << CATALAN;
    leftLanguageList->addItems(languageItems);
    leftLanguageList->sortItems(Qt::AscendingOrder);

    mainProjectConfigVLayout->addWidget(projectBasicPropertiesGroupBox);
    mainProjectConfigVLayout->addSpacing(10);
    mainProjectConfigVLayout->addWidget(projectPathGroupBox);
    mainProjectConfigVLayout->addSpacing(10);
    mainProjectConfigVLayout->addWidget(languagesGroupBox);
    mainProjectConfigVLayout->addLayout(buttonsHLayout);

    setLayout(mainProjectConfigVLayout);
}

/******************************************************************************
 * <Slot>: deselects the language from the project by moving it to the left
 * list.
 ******************************************************************************/
void ProjectSetUpConfig::moveLanguageLeft(){
    if( rightLanguageList->selectedItems().count() == 0 ){
        return;
    }

    QListWidgetItem *item = rightLanguageList->takeItem( rightLanguageList->currentRow() );
    leftLanguageList->addItem( item );
    leftLanguageList->sortItems(Qt::AscendingOrder);

    supportedLangs.sort();
    supportedLangs.removeAt(rightLanguageList->currentRow());

}

/******************************************************************************
 * <Slot>: selects the language to the project by moving it to the right
 * list.
 ******************************************************************************/
void ProjectSetUpConfig::moveLanguageRight(){
    if( leftLanguageList->selectedItems().count() == 0 ){
        return;
    }

    QListWidgetItem *item = leftLanguageList->takeItem( leftLanguageList->currentRow() );
    rightLanguageList->addItem( item );
    rightLanguageList->sortItems(Qt::AscendingOrder);

    supportedLangs << item->text();
}

void ProjectSetUpConfig::changeSupportedLanguages(){
    //TODO reload the actual supported languages

    projectPathGroupBox->setDisabled(true);
}

/******************************************************************************
 * <Slot>: triggers the setup of the project, if all conditions are satisfied:
 * - valid project path
 * - non empty project title
 * - at least one language is selected
 ******************************************************************************/
void ProjectSetUpConfig::okActionSaveConfigurations(){

    bool isDirOK = false;
    bool isProjNameOK = false;
    bool isLangsOK = false;

    QMap<QString,QString> projectProperties = QMap<QString,QString>();

    if(!projectBaseFolder.isEmpty()){
        projectPathLine->setText(projectBaseFolder);
        //inform the main window about the project's base path
        //emit setProjectPath(projectBaseFolder);
        projectProperties.insert(PROJ_CONFIG_BASE_PATH, projectBaseFolder);
        isDirOK = true;
    }
    else{
        Utils::showWarningDialog(this, "Project base directory does not exists or is empty!");
        isDirOK = false;
    }

    QString pName = projectNameLine->text();
    if(!pName.isEmpty()){
        //emit setProjectName(pName);
        projectProperties.insert(PROJ_CONFIG_NAME, pName);
        isProjNameOK = true;
    }
    else{
        Utils::showWarningDialog(this, "Project name could not be empty!");
        isProjNameOK = false;
    }

    if(rightLanguageList->count() != 0){
        isLangsOK = true;
    }
    else{
        Utils::showWarningDialog(this, "You must select at least 1 language!");
        isLangsOK = false;
    }

    if(isLangsOK && isProjNameOK && isDirOK){

        //close config window...
        this->close();

        //save configurations to the project's basic configuration.
        XMLProjectExport *xmlExport = new XMLProjectExport(this);

        bool isSavedProject = false;

        isSavedProject = xmlExport->writeProjectDefinitionXml(projectNameLine->text(),
                                                              projectDescriptionText->toPlainText(),
                                                              projectPathLine->text(),
                                                              CONFIG_XML_FILE,
                                                              supportedLangs);

        if(isSavedProject){
//            progressDialog->setWindowTitle(tr("Creating Project"));
//            progressDialog->setMinimum(0);
//            progressDialog->setMaximum(0);
////            int steps = 0;
////            progressDialog->setValue(steps++);
//            progressLabel->setText(tr("Creating languages..."));
//            progressDialog->show();
//            qApp->processEvents();


//            for(int i=0; i<rightLanguageList->count();i++){
//                QString str = supportedLangs.at(i);
//                if(str.contains(ENGLISH)){
//                    emit setSupportedLanguage(ENGLISH_VALUE);
//                    progressDialog->setValue(steps++);
//                }
//                if(str.contains(SPANISH)){
//                    emit setSupportedLanguage(SPANISH_VALUE);
//                    progressDialog->setValue(steps++);
//                }
//                if(str.contains(CATALAN)){
//                    emit setSupportedLanguage(CATALAN_VALUE);
//                    progressDialog->setValue(steps++);
//                }
//                if(str.contains(ITALIAN)){
//                    emit setSupportedLanguage(ITALIAN_VALUE);
//                    progressDialog->setValue(steps++);
//                }
//            }

            const QString langs = supportedLangs.join(":");

            projectProperties.insert(PROJ_CONFIG_LANGS, langs);

            emit setProjectProperties(projectProperties);

            //tell the main window to create the GUI with the give configs
            progressLabel->setText(tr("Creating subtitle windows..."));
            emit generateGUI();
//            progressDialog->close();
        }

    }
}

/******************************************************************************
 * <Slot>: opens a file dialog to setup the project's base path.
 ******************************************************************************/
void ProjectSetUpConfig::setUpProjectBaseDir(){
    projectBaseFolder = QFileDialog::getExistingDirectory(0,tr("Choose Directory"));
    projectPathLine->setText(projectBaseFolder);
}




