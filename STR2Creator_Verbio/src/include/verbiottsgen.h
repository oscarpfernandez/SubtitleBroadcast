#ifndef VERBIOTTSGEN_H
#define VERBIOTTSGEN_H

#include <windows.h>
#include <stdio.h>
#include <conio.h>

#include "../../libverbio/VerbioTTS.h"

extern "C" {
    VerbioTTS* EngineTTS;
}

class VerbioTTSGen
{
public:
    explicit VerbioTTSGen();
    int verbioTtsAudioGeneration();
private:
    void myTTS_DisconnectFunction(const char *pFailedIP);
    VerbioTTS* EngineTTS;
};



#endif // VERBIOTTSGEN_H
