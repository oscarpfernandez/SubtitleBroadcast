#include "src\include\reproductor.h"

Reproductor::Reproductor(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Reproductor de video"));

    timer = new QTimer(this);
    timer->setInterval(100);
    connect(timer, SIGNAL(timeout()),this, SLOT(calcular_tiempo_total()));

    videoPlayer = new Phonon::VideoPlayer(Phonon::VideoCategory,this);
    //videoPlayer->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    crear_elementos();
    crear_layout();
    resize(250,180);
}


// Slot closeEvent. Para la reproducción si se cierra la ventana.
void Reproductor::closeEvent(QCloseEvent *event)
{
    timer->stop();
    videoPlayer->stop();
    event->accept();
}


// Slot crear_elementos. Crea los elementos del widget.
void Reproductor::crear_elementos()
{

    texto = new QLabel(tr("00:00:00:000 / 00:00:00:000"));

    boton_abrir = new QPushButton(tr("Abrir"));

    boton_abrir->setIcon(QIcon(":/iconos/Eject.png"));
    boton_play = new QPushButton(tr("Play"));
    boton_play->setIcon(QIcon(":/iconos/Play (2).png"));
    boton_pause = new QPushButton(tr("Pause"));
    boton_pause->setIcon(QIcon(":/iconos/Pause.png"));
    boton_stop = new QPushButton(tr("Stop"));
    boton_stop->setIcon(QIcon(":/iconos/Stop (2).png"));

    slider = new Phonon::SeekSlider;
    slider->setMediaObject(videoPlayer->mediaObject());

    connect(boton_abrir, SIGNAL(clicked()),this, SLOT(abrir_archivo()));
    connect(boton_play, SIGNAL(clicked()), this, SLOT(play()));
    connect(boton_pause, SIGNAL(clicked()), this, SLOT(pause()));
    connect(boton_stop, SIGNAL(clicked()), this, SLOT(stop()));
}


// Función crear_layoit. Crea el layout del widget.
void Reproductor::crear_layout()
{
    QHBoxLayout *botones = new QHBoxLayout;
    botones->addWidget(boton_abrir);
    botones->addWidget(boton_play);
    botones->addWidget(boton_pause);
    botones->addWidget(boton_stop);

    QHBoxLayout *tiempo = new QHBoxLayout;
    tiempo->addWidget(slider);
    tiempo->addWidget(texto);

    QVBoxLayout *controles = new QVBoxLayout;
    controles->addLayout(tiempo);
    controles->addLayout(botones);

    video = new QHBoxLayout;
    video->addWidget(videoPlayer);

    mainLayout = new QVBoxLayout;
    mainLayout->addLayout(video);
    mainLayout->addLayout(controles);

    setLayout(mainLayout);
}


// Slot abrir_archivo. Abre una ventana para cargar un archivo de video y luego genera los objetos multimedia necesarios.
void Reproductor::abrir_archivo()
{
    Nombre_Archivo = QFileDialog::getOpenFileName(this,
                               tr("Abrir Video"), ".",
                               tr("Archivo de video (*.*)"));

    videoPlayer->play(Nombre_Archivo);
    videoPlayer->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    timer->start();
}


// Slot calcular_tiempo_total. Calcula los tiempos de la reproducción (total y actual)
void Reproductor::calcular_tiempo_total()
{
    ms_actual = videoPlayer->currentTime();
    ms_total = videoPlayer->totalTime();

    sec = ms_actual / 1000;
    min = sec / 60;
    hour = min / 60;
    msec = ms_actual;

    QTime actualTime(hour%60, min%60, sec%60, msec%1000);

    sec = ms_total / 1000;
    min = sec / 60;
    hour = min / 60;
    msec = ms_total;

    QTime stopTime(hour%60, min%60, sec%60, msec%1000);

    QString hora;
    hora = actualTime.toString("hh:mm:ss:zzz");

    hora += " / " + stopTime.toString("hh:mm:ss:zzz");
    texto->setText(hora);
}


// Slot play. Pone en marcha la reproducción de video.
void Reproductor::play()
{
    videoPlayer->play();
}


// Slot pause. Pone en pausa la reproducción.
void Reproductor::pause()
{
    videoPlayer->pause();
}


// Slot stop. Para definitivamente la reproducción.
void Reproductor::stop()
{
    videoPlayer->stop();
}



