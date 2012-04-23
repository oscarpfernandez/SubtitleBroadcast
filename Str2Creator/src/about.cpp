#include "about.h"

/******************************************************************************
 * This class implements the traditional "about" dialog with some informations
 * about the application: versions, build dates, framework used and copyright
 * warning.
 ******************************************************************************/

About::About(QWidget *parent) : QDialog(parent)
{
    QString windowText = QString("About ");
    windowText.append(APP_NAME);
    windowText.append(" (v.");
    windowText.append(APP_VERSION);
    windowText.append(")");

    setWindowTitle(windowText);
    setWindowOpacity(0.95);
    setWindowModality(Qt::ApplicationModal);

    createElements();

    setLayout(mainVBoxLayout);
    setFixedSize(sizeHint().width(),550);

}

/******************************************************************************
 * Creates all the elements shown on the about dialog.
 ******************************************************************************/
void About::createElements()
{
    mainVBoxLayout = new QVBoxLayout;
    buttonHBoxLayout = new QHBoxLayout;

    imageLabel = new QLabel();
    imageLabel->setPixmap(QPixmap(":/icons/splashscreen.png"));

    QPalette pal;
    QColor bgColor("white");
    QColor fgColor("blue");
    pal.setColor(QPalette::Base, bgColor);
    pal.setColor(QPalette::Text, fgColor);

    QPushButton *button = new QPushButton();
    button->setText("Ok");
    button->setMaximumWidth(90);
    connect(button, SIGNAL(clicked()), this, SLOT(closeDialog()));

    buttonHBoxLayout->addWidget(button);

    QString textToShow = QString();
    textToShow.append("<DIV ALIGN=CENTER><p>Based on QT Framework version: ");
    textToShow.append(QString(qVersion()));
    textToShow.append("</p>");
    textToShow.append("<p>Built on ");
    textToShow.append(QDate::currentDate().toString());
    textToShow.append(" at ");
    textToShow.append(QTime::currentTime().toString());
    textToShow.append("</p>");
    textToShow.append("<p>Copyright 2011-2012 - All rights reserved.</p>");
    textToShow.append("</p>");
    textToShow.append("<p><a href=\"http://www.uab.cat/\">Universidad Autonoma de Barcelona</a></p></DIV>");

    textEdit = new QTextEdit(this);
    textEdit->setHtml(textToShow);
    textEdit->setReadOnly(true);
    textEdit->setCurrentFont(QFont("Arial",14, QFont::Bold));
    textEdit->setAlignment(Qt::AlignCenter);
    textEdit->setPalette(pal);
    mainVBoxLayout->addWidget(imageLabel);
    mainVBoxLayout->addWidget(textEdit);
    mainVBoxLayout->addLayout(buttonHBoxLayout);
}

void About::closeDialog()
{
    this->close();
}
