#include "audiogeneratorconfig.h"

/******************************************************************************
 * Description:
 * This class is a front-end to the audio generation of subtitles and audio
 * descriptions. From here the user can choose what languages must be considered
 * for the automatic audio generation using the TTS system.
 ******************************************************************************/

AudioGeneratorConfig::AudioGeneratorConfig(
        QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Automatic Audio Generation Setup");
    setModal(true);
    setMinimumSize(350,150);
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
void AudioGeneratorConfig::createGuiElements()
{
    mainProjectConfigVLayout = new QVBoxLayout;
    projectPathConfigLineHLayout = new QHBoxLayout;
    languagesConfigVLayout = new QVBoxLayout;

    //Languages configuration
    languagesGroupBox = new QGroupBox(tr("Select languages"));
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

    connect( okButton,     SIGNAL(clicked()), this, SLOT(okStartAudioGeneration()));
    connect( cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    connect( leftButton,   SIGNAL(clicked()), this, SLOT(moveLanguageLeft()) );
    connect( rightButton,  SIGNAL(clicked()), this, SLOT(moveLanguageRight()) );

    leftLanguageList->sortItems(Qt::AscendingOrder);

    mainProjectConfigVLayout->addWidget(languagesGroupBox);
    mainProjectConfigVLayout->addLayout(buttonsHLayout);

    setLayout(mainProjectConfigVLayout);
}

/******************************************************************************
 * <Slot>
 * Deselects the language by moving it to the left list.
 ******************************************************************************/
void AudioGeneratorConfig::moveLanguageLeft(){
    if( rightLanguageList->selectedItems().count() == 0 ){
        return;
    }

    QListWidgetItem *item = rightLanguageList->takeItem( rightLanguageList->currentRow() );
    leftLanguageList->addItem( item );
    leftLanguageList->sortItems(Qt::AscendingOrder);

    langsToGenerate.sort();
    langsToGenerate.removeAt(rightLanguageList->currentRow());

}

/******************************************************************************
 * <Slot>
 * Selects the language by moving it to the right list.
 ******************************************************************************/
void AudioGeneratorConfig::moveLanguageRight(){
    if( leftLanguageList->selectedItems().count() == 0 ){
        return;
    }

    QListWidgetItem *item = leftLanguageList->takeItem( leftLanguageList->currentRow() );
    rightLanguageList->addItem( item );
    rightLanguageList->sortItems(Qt::AscendingOrder);

    langsToGenerate << item->text();
}

void AudioGeneratorConfig::setSupportedLanguages(QStringList &langsSupported){
    leftLanguageList->clear();
    rightLanguageList->clear();
    leftLanguageList->addItems(langsSupported);
}

/******************************************************************************
 * <Slot>
 * Triggers the setup of the project, if all conditions are satisfied:
 * - at least one language is selected
 ******************************************************************************/
void AudioGeneratorConfig::okStartAudioGeneration(){

    bool isLangsOK = false;


    if(rightLanguageList->count() != 0){
        isLangsOK = true;
    }
    else{
        Utils::showWarningDialog(this, "You must select at least 1 language!");
        isLangsOK = false;
    }

    if(isLangsOK){
        close();//close this menu
        emit startTTSAudioGeneration(langsToGenerate);
    }


}
