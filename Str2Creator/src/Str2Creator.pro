#-------------------------------------------------
#
# Project created by QtCreator 2011-03-28T14:51:41
#
#-------------------------------------------------

QT       += core gui

TARGET = Str2Creator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    sleeper.cpp \
    subtitles.cpp \
    audiorecorder.cpp \
    audiowav.cpp \
    reproductor.cpp \
    audioplayer.cpp \
    projectsetupconfig.cpp \
    subtitlelineviewer.cpp \
    xmlprojectexport.cpp \
    utils.cpp \
    festivalaudiogenerator.cpp \
    audiogeneratorconfig.cpp \
    about.cpp

HEADERS  += mainwindow.h \
    sleeper.h \
    subtitles.h \
    audiowav.h \
    constansts.h \
    reproductor.h \
    audioplayer.h \
    projectsetupconfig.h \
    audiorecorder.h \
    subtitlelineviewer.h \
    xmlprojectexport.h \
    basetypes.h \
    utils.h \
    festivalaudiogenerator.h \
    audiogeneratorconfig.h \
    about.h


RESOURCES += Resources.qrc

QT += multimedia
QT += phonon
QT += xml

INCLUDEPATH += "/usr/include/speech_tools"
INCLUDEPATH += "/usr/include/festival"
#LIBS += -L"/usr/lib/speech_tools/lib/libestools.a"
#LIBS += -L"/usr/lib/speech_tools/lib/libestbase.a"
#LIBS += -L"/usr/lib/speech_tools/lib/libeststring.a"
LIBS += -L"/usr/lib/speech_tools/lib"
LIBS += -lestools
LIBS += -lestbase
LIBS += -leststring
#LIBS += -L"/usr/lib/libFestival.a"
LIBS += -L"/usr/lib"
LIBS += -lFestival
LIBS += -lncurses
LIBS += -lesd
LIBS += -lpulse-simple

OTHER_FILES += \
