#ifndef FESTIVALAUDIOGENERATOR_H
#define FESTIVALAUDIOGENERATOR_H

#include <QObject>

#include "/usr/include/festival/festival.h"
#include "/usr/include/speech_tools/EST_Wave.h"

class FestivalAudioGenerator {

public:
    explicit FestivalAudioGenerator(int schemeHeapSize=210000,
                                    int loadFestivalInitFiles=1);
    void setVoiceName(QString &voiceName);
    int generateAudioFile(QString *textToSay,
                          QString *filePath,
                          QString *fileType);

};

#endif // FESTIVALAUDIOGENERATOR_H
