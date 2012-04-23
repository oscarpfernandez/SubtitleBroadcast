#include "languagedialog.h"
#include <QPushButton>
#include <QGridLayout>
#include <QListWidget>

LanguageDialog::LanguageDialog() : QDialog()
{
    QPushButton *left, *right;
    QGridLayout *layout = new QGridLayout( this );
    layout->addWidget( left = new QPushButton( "<<" ), 0, 1 );
    layout->addWidget( right = new QPushButton( ">>" ), 1, 1 );
    layout->addWidget( leftLanguageList = new QListWidget, 0, 0, 3, 1 );
    layout->addWidget( rightLanguageList = new QListWidget, 0, 2, 3, 1 );
    connect( left, SIGNAL(clicked()), this, SLOT(moveLeft()) );
    connect( right, SIGNAL(clicked()), this, SLOT(moveRight()) );
    QStringList items;
    items << "Spanish" << "Portuguese" << "English" << "Catalan";
    leftLanguageList->addItems(items);
}

void LanguageDialog::moveLeft()
{

}

void LanguageDialog::moveRight()
{

}

