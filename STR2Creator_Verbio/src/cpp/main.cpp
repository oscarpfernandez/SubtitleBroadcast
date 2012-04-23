#include "src\include\mainwindow.h"
#include "src\include\sleeper.h"
#include "src\include\splashscreen.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    mainwindow mainWin;

//    mainWin.setStyleSheet("QCheckBox, QComboBox, QLineEdit, QListViewItem { background-color: yellow;}");

    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/icons/splashscreen.png"));
    splash->show();

    Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;

    splash->showMessage(QObject::tr("Loading Application..."),
                        topRight, Qt::white);
    //establishConnections();

    Sleeper::sleep(3000);

    mainWin.showMaximized();

    splash->finish(&mainWin);
    delete splash;


    return app.exec();

}
