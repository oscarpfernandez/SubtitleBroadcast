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

  /*  if (argc < 2) {
    std::cout << "usage: " << argv[0] << " <filename>\n";
    exit(-1);
    } */

  QSplashScreen *splash = new QSplashScreen;
  splash->setPixmap(QPixmap(":/icons/splashbc.png"));
  splash->show();

//  Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;

  splash->showMessage(QObject::tr("Loading Application..."),
                      Qt::AlignLeft | Qt::AlignTop, Qt::white);

  Sleeper::sleep(3000);

  mainwin.showMaximized();

  splash->finish(&mainwin);
  delete splash;


  //Displist *dsplist = new Displist(0);
  //SubtitlesLaunchDialog *subListDiag = new SubtitlesLaunchDialog;

//  if (dsplist->getFailInit() == 1) {
//    std::cout << "application init: socket creation failure\n";
//    exit(-2);
//  }

  //subListDiag->show();

  return app.exec();
}
