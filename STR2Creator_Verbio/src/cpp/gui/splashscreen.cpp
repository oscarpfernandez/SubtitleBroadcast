#include "splashscreen.h"

SplashScreen::SplashScreen(QWidget *widget, QPixmap *pixmap)
{
    //Splash Screen Configuration...
    splash = new QSplashScreen(pixmap);
    topRight = Qt::AlignRight | Qt::AlignTop;
    splash->setPixmap(pixmap);
    splash->show();
    splash->showMessage(QObject::tr("Setting up the main window..."), topRight, Qt::white);

    splash->showMessage(QObject::tr("Loading Projects..."), topRight, Qt::white);
    //loadProjects();

    splash->showMessage(QObject::tr("Loading application..."), topRight, Qt::white);
    //finishingLoading();

    splash->finish(&mainWin);

}

void SplashScreen::showNewMessage(const QString message)
{
    splash->showMessage(tr(message), topRight, Qt::white);
}

void SplashScreen::show()
{
    splash->show();
}

void SplashScreen::disable(QWidget *widget){
    splash->finish(widget);
    delete splash;
}
