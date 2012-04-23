#-------------------------------------------------
#
# Project created by QtCreator 2011-12-05T15:12:18
#
#-------------------------------------------------

QT       += core gui

TARGET = STR2Creator_Verbio
TEMPLATE = app


SOURCES += src\cpp\main.cpp\
        src\cpp\gui\mainwindow.cpp \
    src\cpp\misc\sleeper.cpp \
    src\cpp\gui\subtitles.cpp \
    src\cpp\gui\audiorecorder.cpp \
    src\cpp\gui\audiowav.cpp \
    src\cpp\gui\reproductor.cpp \
    src\cpp\gui\audioplayer.cpp \
    src\cpp\gui\projectsetupconfig.cpp \
    src\cpp\gui\subtitlelineviewer.cpp \
    src\cpp\xml\xmlprojectexport.cpp \
    src\cpp\misc\utils.cpp \
    src\cpp\gui\audiogeneratorconfig.cpp \
    src\cpp\gui\about.cpp \
    src/cpp/tts/verbiottsgen.cpp

HEADERS  += src\include\mainwindow.h \
    src\include\sleeper.h \
    src\include\subtitles.h \
    src\include\audiowav.h \
    src\include\constansts.h \
    src\include\reproductor.h \
    src\include\audioplayer.h \
    src\include\projectsetupconfig.h \
    src\include\audiorecorder.h \
    src\include\subtitlelineviewer.h \
    src\include\xmlprojectexport.h \
    src\include\basetypes.h \
    src\include\utils.h \
    src\include\audiogeneratorconfig.h \
    src\include\about.h \
    src\include\verbiottsgen.h

INCLUDEPATH += "./libverbio"

LIBS += -L"./libverbio/verbiolib.lib"
LIBS += -L"./libverbio/voxlib.lib"

RESOURCES += Resources.qrc \
    Resources.qrc

QT += multimedia
QT += phonon
QT += xml









