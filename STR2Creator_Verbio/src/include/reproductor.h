#ifndef REPRODUCTOR_H
#define REPRODUCTOR_H

#include <QDialog>
#include <Phonon>
#include <QtGui>
#include <QCloseEvent>

class Reproductor : public QDialog
{
    Q_OBJECT

public:
    Reproductor(QWidget *parent = 0);

protected:

    void closeEvent(QCloseEvent *event);

private slots:

    void abrir_archivo();
    void calcular_tiempo_total();
    void play();
    void pause();
    void stop();

private:

    void crear_layout();
    void crear_elementos();

    QPushButton *boton_abrir;
    QPushButton *boton_play;
    QPushButton *boton_pause;
    QPushButton *boton_stop;

    QLabel *texto;
    QString Nombre_Archivo;

    QHBoxLayout *video;
    QVBoxLayout *mainLayout;

    QTime *tiempo_total;
    QTimer *timer;

    long ms_actual;
    long ms_total;

    int sec;
    int min;
    int hour;
    int msec;

    Phonon::SeekSlider *slider;
    Phonon::MediaObject *mediaObjectPlayer;
    Phonon::VideoPlayer *videoPlayer;
};

#endif // REPRODUCTOR_H
