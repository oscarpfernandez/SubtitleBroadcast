#ifndef AUDIOWAV_H
#define AUDIOWAV_H

#include <QFile>
#include <QtCore>
#include <QtMultimedia>

class Audiowav : public QFile
{
public:
    Audiowav(const QString & name, const QAudioFormat & format);
    bool open();
    void close();

private:
    void writeAudioFileHeader();
    bool validateAudioFormat();
    QAudioFormat format;

};

#endif // AUDIOWAV_H
