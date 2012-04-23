#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QString>
#include <QSplashScreen>

class SplashScreen
{
public:
    SplashScreen(QWidget *widget, QPixmap pixmap);
    void showNewMessage(const QString message);
    void show();
    void disable();

private:
    QSplashScreen *splash;
    Qt::Alignment topRight;
};

#endif // SPLASHSCREEN_H
