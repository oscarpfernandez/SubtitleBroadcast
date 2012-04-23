#include "festivalaudiogenerator.h"


/******************************************************************************
 * Descritption:
 * The purpose of this class is to provide speech synthesis functionalities
 * generating an audio file from some text in a given language.
 * This implementation integrates the Festival TtS libraries.
 * For more information go to [http://www.cstr.ed.ac.uk/projects/festival]
 ******************************************************************************/

FestivalAudioGenerator::FestivalAudioGenerator(int schemeHeapSize,
                                               int loadFestivalInitFiles)
{
    qDebug("ENTRY FestivalAudioGenerator ");

    //210000 is the default scheme heap size
    int heap_size = schemeHeapSize;

    //1: we want the festival init files loaded
    int load_init_files = loadFestivalInitFiles;

    festival_initialize(load_init_files,heap_size);

    // Say simple file
    //festival_say_file("/etc/motd");

    qDebug("EXIT FestivalAudioGenerator ");
}

/******************************************************************************
 * Configures the voice (and implicitly the associated language)
 * Args:
 * - voiceName - the voice's name accordingly to the Festival TTS sintethyzer.
 ******************************************************************************/
void FestivalAudioGenerator::setVoiceName(QString &voiceName)
{
    qDebug("ENTRY FestivalAudioGenerator::setVoiceName");
    if(voiceName != 0 && voiceName.count()!=0){
        // e.g. "(voice_ked_diphone)"
        festival_eval_command(EST_String(voiceName.toAscii().data()));
        const QString v(voiceName);
        qDebug("\tSelected voice: "+v.toAscii());
    }

    qDebug("EXIT FestivalAudioGenerator::setVoiceName");
}

/******************************************************************************
 * Generates one audio file
 * Args:
 * - QString &textToSay - the text that is going to be processed by the TTS.
 * - QString &filePath - the path to store the file.
 * - QString &fileType - the type of the file (tipically a riff)
 ******************************************************************************/
int FestivalAudioGenerator::generateAudioFile(QString *textToSay,// "something funny"
                                              QString *filePath, // "/tmp/wave.wav"
                                              QString *fileType) // "riff"

{
    if(textToSay==0 || filePath==0 || fileType==0)
    {
        //this should not happen, but... just in case...
        qDebug(">>FestivalAudioGenerator::generateAudioFile received parameters with null pointers...returning");
        return -1;
    }

    qDebug("ENTRY FestivalAudioGenerator::generateAudioFile");

    const QString t2say(*textToSay);
    const QString path(*filePath);
    const QString type(*fileType);

    qDebug("\t-Text: "+t2say.toAscii()+
           "\n\t-Path: "+path.toAscii()+
           "\n\t-file type: "+type.toAscii());

    EST_Wave wave;
    wave.set_sample_rate(9000);

    // Convert to a waveform
    festival_text_to_wave(EST_String(textToSay->toAscii().data()),wave);
    wave.save(EST_String(filePath->toAscii().data()),EST_String(fileType->toAscii().data()));

    // festival_say_file puts the system in async mode so we better
    // wait for the spooler to reach the last waveform before exiting
    // This isn't necessary if only festival_say_text is being used (and
    // your own wave playing stuff)
    festival_wait_for_spooler();

    qDebug("EXIT FestivalAudioGenerator::generateAudioFile");
    return 0;
}

