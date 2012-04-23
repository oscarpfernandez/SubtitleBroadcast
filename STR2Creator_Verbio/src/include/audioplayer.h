#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QtGui>
#include <QObject>
#include <QDialog>
#include <Phonon>

class AudioPlayer : public QDialog
{
    Q_OBJECT

public:
    AudioPlayer(QWidget *parent = 0);
    QString Directorio_Trabajo;


protected:

    void closeEvent(QCloseEvent *event);

signals:
    void cargar_audiodescriptor(int indice_,QString nombre_, QString delay_);
    void cargar_audiodoblado(int indice_,QString nombre_);

private slots:

    void calcular_tiempos();
    void cargar_archivo();
    void guardar_archivo();
    void reproducir_archivo();
    void stop();
    void pause();
    void modo_audiodescriptor();
    void modo_audiodoblado();

public slots:
    void definir_directorios(QString &Dir_AUD, QString &Dir_DOB);

private:

    void crear_elementos();
    void crear_layout();

    QLabel *texto1;
    QLabel *texto2;
    QLabel *texto3;
    QLabel *tiempo;
    QLabel *linea_blanco;

    QPushButton *boton_play;
    QPushButton *boton_stop;
    QPushButton *boton_pause;
    QPushButton *boton_cargar;
    QPushButton *boton_explorar;

    QLineEdit *nombre_pista;
    QLineEdit *delay;

    QString file_path;
    QSpinBox *indice;

    QLabel *texto5;
    QComboBox *idiomas;
    QStringList lista_idiomas;

    QCheckBox *check_audiodes;
    QCheckBox *check_audiodob;

    QString Directorio_AUD;
    QString Directorio_DOB;

    QLCDNumber *relojLCD;
    QString tiempo_total;
    QTimer *timer;

    long ms_actual;
    long ms_total;

    int sec;
    int min;
    int hour;
    int msec;

    bool parado;

    Phonon::MediaObject *mediaObject;
    Phonon::AudioOutput *audioOutput;
    Phonon::SeekSlider *slider;
};

#endif // AUDIOPLAYER_H
