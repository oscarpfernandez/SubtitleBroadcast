/******************************************************************************
 * STR2 Content Broadcast Server
 *
 * Developed by Oscar Lopes as the final project of the Multimedia Technologies
 * Master, and sponsored by CAIAC (caiac.uab.cat)
 *
 * (C)2011 [Universidad Autònoma de Barcelona]
 ******************************************************************************/

#include <QtGui>
#include "displist.h"
#include <iostream>
#include <stdlib.h>
#include "sleeper.h"
#include "mainwindow.h"
#include "subtitleslaunchdialog.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  //SubtitlesLaunchDialog subldiag;
  mainwindow mainwin;

  QSplashScreen *splash = new QSplashScreen;
  splash->setPixmap(QPixmap(":/icons/splashbc.png"));
  splash->show();

  splash->showMessage(QObject::tr("Loading Application..."),
                      Qt::AlignLeft | Qt::AlignTop, Qt::white);

  Sleeper::sleep(3000);

  mainwin.showMaximized();

  splash->finish(&mainwin);
  delete splash;

  return app.exec();
}
