#include <QtGui>
#include <QCoreApplication>
#include "src\include\audioplayer.h"
#include "src\include\constansts.h"

AudioPlayer::AudioPlayer(QWidget *parent)
    :QDialog(parent)
{
    //resize(420,60);
    setWindowTitle(tr("Audio Player"));

    timer = new QTimer(this);
    timer->setInterval(100);
    connect(timer, SIGNAL(timeout()),this, SLOT(calcular_tiempos()));

    relojLCD = new QLCDNumber(this);
    relojLCD->setSegmentStyle(QLCDNumber::Filled);
    relojLCD->setNumDigits(9);
    relojLCD->display(tr("00:00.000"));
    relojLCD->setMinimumWidth(80);
    relojLCD->setMinimumHeight(50);

    audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    mediaObject = new Phonon::MediaObject(this);

    Phonon::createPath(mediaObject, audioOutput);

    crear_elementos();
    crear_layout();
}


// Slot closeEvent. Stop the audio player and closes window.
void AudioPlayer::closeEvent(QCloseEvent *event)
{
    timer->stop();
    mediaObject->stop();
    event->accept();
}


// Funcin crear_elementos. Crea todos los elementos del widget.
void AudioPlayer::crear_elementos()
{
    texto1 = new QLabel(tr("Track"));
    texto2 = new QLabel(tr("Subtitle Index"));
    texto3 = new QLabel(tr("Delay"));
    linea_blanco = new QLabel(tr(" "));

    boton_explorar = new QPushButton;
    boton_explorar->setIcon(QIcon(":/icons/eject.png"));
    boton_explorar->setIconSize(QSize(30,30));
    boton_play = new QPushButton;
    boton_play->setIcon(QIcon(":/icons/play.png"));
    boton_play->setIconSize(QSize(30,30));
    boton_stop = new QPushButton;
    boton_stop->setIcon(QIcon(":/icons/stop.png"));
    boton_stop->setIconSize(QSize(30,30));
    boton_pause = new QPushButton;
    boton_pause->setIcon(QIcon(":/icons/pause.png"));
    boton_pause->setIconSize(QSize(30,30));
    boton_cargar = new QPushButton(tr("Load"));

    indice = new QSpinBox;
    indice->setMinimum(1);
    indice->setMaximum(5000);
    indice->setFixedWidth(indice->sizeHint().width());
    indice->setAlignment(Qt::AlignLeft);

    check_audiodes = new QCheckBox(tr("Description"));
    check_audiodes->setChecked(true);
    check_audiodob = new QCheckBox(tr("Subtitle"));

    texto5 = new QLabel(tr("Language: "));
    idiomas = new QComboBox(this);
    lista_idiomas<< SPANISH << ENGLISH << CATALAN << ITALIAN;
    for(int i = 0; i < lista_idiomas.size(); i++ )
        idiomas->addItem(lista_idiomas.at(i));

    connect(boton_explorar, SIGNAL(clicked()),this, SLOT(cargar_archivo()));
    connect(boton_play, SIGNAL(clicked()),this, SLOT(reproducir_archivo()));
    connect(boton_stop, SIGNAL(clicked()),this, SLOT(stop()));
    connect(boton_pause,SIGNAL(clicked()),this,SLOT(pause()));
    connect(boton_cargar, SIGNAL(clicked()),this, SLOT(guardar_archivo()));
    connect(check_audiodes,SIGNAL(clicked()),this,SLOT(modo_audiodescriptor()));
    connect(check_audiodob,SIGNAL(clicked()),this,SLOT(modo_audiodoblado()));

    nombre_pista = new QLineEdit;
    nombre_pista->setReadOnly(true);
    delay = new QLineEdit;
    delay->setText(tr("00:00,000"));
    delay->setAlignment(Qt::AlignLeft);
    delay->setMaximumWidth(90);

    slider = new Phonon::SeekSlider;
    slider->setMediaObject(mediaObject);
}


// Funcin crear_layout. Genera el layout del widget.
void AudioPlayer::crear_layout()
{
    QHBoxLayout *reloj_layout = new QHBoxLayout;
    reloj_layout->addWidget(relojLCD);

    QHBoxLayout *primera_linea_reproductor = new QHBoxLayout;
    primera_linea_reproductor->addWidget(texto1);
    primera_linea_reproductor->addWidget(nombre_pista);

    QHBoxLayout *segunda_linea_reproductor = new QHBoxLayout;
    segunda_linea_reproductor->addWidget(boton_explorar);
    segunda_linea_reproductor->addWidget(boton_play);
    segunda_linea_reproductor->addWidget(boton_pause);
    segunda_linea_reproductor->addWidget(boton_stop);

    QHBoxLayout *tercera_linea_reproductor = new QHBoxLayout;
    tercera_linea_reproductor->addWidget(slider);

    QHBoxLayout *segunda_linea = new QHBoxLayout;
    segunda_linea->addWidget(texto2);
    segunda_linea->addWidget(indice);

    QHBoxLayout *tercera_linea = new QHBoxLayout;
    tercera_linea->addWidget(check_audiodes);
    tercera_linea->addWidget(check_audiodob);

    QHBoxLayout *cuarta_linea = new QHBoxLayout;
    cuarta_linea->addWidget(texto3);
    cuarta_linea->addWidget(delay);

    QHBoxLayout *sexta_linea = new QHBoxLayout;
    sexta_linea->addWidget(texto5);
    sexta_linea->addWidget(idiomas);

    QHBoxLayout *quinta_linea = new QHBoxLayout;
    quinta_linea->addWidget(boton_cargar);

    QVBoxLayout *opciones_layout = new QVBoxLayout;
    opciones_layout->addLayout(segunda_linea);
    opciones_layout->addLayout(tercera_linea);
    opciones_layout->addLayout(cuarta_linea);
    opciones_layout->addLayout(sexta_linea);
    opciones_layout->addLayout(quinta_linea);

    QVBoxLayout *reproductor_layout = new QVBoxLayout;
    reproductor_layout->addLayout(primera_linea_reproductor);
    reproductor_layout->addWidget(linea_blanco);
    reproductor_layout->addLayout(reloj_layout);
    reproductor_layout->addLayout(tercera_linea_reproductor);
    reproductor_layout->addLayout(segunda_linea_reproductor);


    QGroupBox *reproductorBox = new QGroupBox(tr("Audio Player"));
    reproductorBox->setLayout(reproductor_layout);

    QGroupBox *opcionesBox = new QGroupBox(tr("Options"));
    opcionesBox->setLayout(opciones_layout);


    QGridLayout *grid = new QGridLayout;
    grid->addWidget(reproductorBox, 0, 0);
    grid->addWidget(opcionesBox, 0, 1);

    setLayout(grid);
    setFixedSize(grid->sizeHint());
}


// Slot cargar_archivo. Carga un archivo de audio cualquiera.
void AudioPlayer::cargar_archivo()
{
    QString nombre_archivo = QFileDialog::getOpenFileName(this,
                                                          tr("Load Audio File"), ".", tr("Archivo de audio (*.*)"));
    QStringList lista_path;
    lista_path = nombre_archivo.split("/");
    nombre_pista->setText(lista_path.at(lista_path.size()-1));
    file_path = nombre_archivo;
    mediaObject->setCurrentSource(file_path);
    parado = true;
    timer->start();
}


// Slot reproducir_archivo. Reproduce el archivo de audio que se haya cargado.
void AudioPlayer::reproducir_archivo()
{

    parado = false;
    if(!timer->isActive())
        timer->start();
    mediaObject->play();
}


// Slot stop. Para la reproduccin.
void AudioPlayer::stop()
{
    parado = true;
    timer->stop();
    mediaObject->stop();
    relojLCD->display(tiempo_total);
    if (nombre_pista->text().isEmpty())
        relojLCD->display(tr("00:00.000"));
}


// Slot pause. Para momentaneamente la reproduccin.
void AudioPlayer::pause()
{
    mediaObject->pause();
}


// Slot calcular_tiempos. Calcula el tiempo de reproduccin para cada instante.
void AudioPlayer::calcular_tiempos()
{
    if (parado==false){
        ms_actual = mediaObject->currentTime();
        sec = ms_actual / 1000;
        min = sec / 60;
        hour = min / 60;
        msec = ms_actual;
        QTime actualTime(hour%60, min%60, sec%60, msec%1000);
        QString hora;
        hora = actualTime.toString("mm:ss.zzz");
        relojLCD->display(hora);
    }

    if (parado==true){
        ms_total = mediaObject->totalTime();
        sec = ms_total / 1000;
        min = sec / 60;
        hour = min / 60;
        msec = ms_total;
        QTime stopTime(hour%60, min%60, sec%60, msec%1000);
        tiempo_total = stopTime.toString("mm:ss.zzz");
        relojLCD->display(tiempo_total);
    }
}


// Slot guardar_archivo. Carga el archivo de audio en la tabla de subttulos.
void AudioPlayer::guardar_archivo()
{
    QString idioma = idiomas->currentText().left(3);
    QString marca1("<>");
    marca1.insert(1,idioma);
    QString marca2("</>");
    marca2.insert(2,idioma);

    QString nombre_tabla;
    nombre_tabla = marca1;
    nombre_tabla.append(nombre_pista->text());
    nombre_tabla.append(marca2);

    QString Archivo_nuevo;

    if(!nombre_pista->text().isEmpty())
    {
        if (check_audiodes->isChecked()){
            emit cargar_audiodescriptor(indice->value(),nombre_tabla,delay->text());
            Archivo_nuevo = Directorio_AUD;
        }

        if (check_audiodob->isChecked()){
            emit cargar_audiodoblado(indice->value(), nombre_tabla);
            Archivo_nuevo = Directorio_DOB;
        }

        Archivo_nuevo.append(nombre_pista->text());
        QFile::copy(file_path,Archivo_nuevo);
    }
}


// Slot definir_directorios. Define los directorios donde han de copiarse las pistas de audio.
void AudioPlayer::definir_directorios(QString &Dir_AUD, QString &Dir_DOB)
{
    Directorio_AUD = Dir_AUD;
    Directorio_DOB = Dir_DOB;
}


// Slot modo_audiodescriptor. Cambia las opciones si se va a guardar un audiodescriptor.
void AudioPlayer::modo_audiodescriptor()
{
    if (check_audiodob->isChecked()==true)
        check_audiodob->setChecked(false);
}


// Slot modo_audiodoblad. Cambia las opcines si se va a guardar audio doblado.
void AudioPlayer::modo_audiodoblado()
{
    if (check_audiodes->isChecked()==true)
        check_audiodes->setChecked(false);
}


