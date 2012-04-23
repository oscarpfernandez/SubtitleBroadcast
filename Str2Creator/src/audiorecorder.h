#ifndef AUDIORECORDER_H
#define AUDIORECORDER_H

#include <QDialog>
#include <QErrorMessage>
#include <QLCDNumber>
#include <Phonon>
#include <QAudioDeviceInfo>
#include <QGroupBox>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QFile>
#include <QAudioOutput>
#include <QCheckBox>
#include <QEventLoop>
#include "audiowav.h"
#include "constansts.h"


class AudioRecorder : public QDialog
{
    Q_OBJECT

public:
    AudioRecorder(QWidget *parent = 0, QMainWindow *window = 0);
    QString Directorio_Trabajo;
    QString Directorio_AUD;
    QString Directorio_DOB;
    void hideDockableAudioRecorder();

protected:
    void closeEvent(QCloseEvent *event);

signals:
    void cargar_audiodescriptor(int indice_,QString nombre_, QString delay_);
    void cargar_audiodoblado(int indice_,QString nombre_);

private slots:
    void crono();
    void crono_reproductor();
    void grabar();
    void reproducir();
    void parar();
    void modo_audiodescriptor();
    void modo_audiodoblado();
    void cambio_frecuencia();

public slots:
    void inicializar(QString &dir_AUD, QString &dir_DOB);
    void showDockableAudioRecorder();

private:
    QMainWindow *mainwin;
    void createLayout();
    void createElements();

    QLabel *texto1;
    QLabel *texto2;
    QLabel *texto3;
    QLabel *texto4;

    QString hora;

    QGroupBox *player;
    QSpinBox *indice;
    QLineEdit *retraso;
    QPushButton *boton_play;
    QPushButton *boton_rec;
    QPushButton *boton_pause;
    QPushButton *boton_stop;
    QComboBox *seleccion_entrada;
    QCheckBox *check_audiodes;
    QCheckBox *check_audiodob;

    QComboBox *frecuencias_muestreo;
    QList<int> frecuencias;

    QLabel *texto5;
    QComboBox *idiomas;
    QStringList lista_idiomas;

    QAudioOutput *audio;
    QFile inputFile;

    QLCDNumber *relojLCD;
    QTimer *reloj;
    QTime cronometro;
    QTime hora_ini;

    bool reproduccion;

    QTime hora_anterior;
    QTime hora_actual;

    long ms_actual;
    long ms_actual_anterior;
    int sec;
    int min;
    int msec;
    int hour;

    int frecuencia_seleccionada;

    bool stop;
    QEventLoop loop;

    Phonon::MediaObject *mediaObject;
    Phonon::AudioOutput *audioOutput;
    Phonon::MediaObject *mediaObject2;
    Phonon::VideoWidget *videowidget;

    Audiowav *archivo_salida;

    QDockWidget *audioRecorderDockWidget;
};

#endif // AudioRecorder_H
