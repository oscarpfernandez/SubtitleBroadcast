#include "audiowav.h"

Audiowav::Audiowav(const QString & name, const QAudioFormat & format_)
    : QFile(name), format(format_)
{
}

// Validación de Formato para archivos WAV.
bool Audiowav::validateAudioFormat()
{
    if ((format.sampleSize()>=8) && (format.sampleType() == QAudioFormat::SignedInt)
         && (format.byteOrder() == QAudioFormat::LittleEndian))
        return true;
    else
        return false;
}

// Creación del archivo WAV.
bool Audiowav::open()
{
    if (!validateAudioFormat())
    {
        setErrorString("Formato no aceptado para PCM/WAV");
        return false;
    }
    else
    {
        if (!QFile::open(ReadWrite | Truncate))
            return false;
        writeAudioFileHeader();
        return true;
    }
}

// Escritura del encabezado del archivo WAV.
void Audiowav::writeAudioFileHeader()
{
    QDataStream out(this);

    out.setByteOrder(QDataStream::BigEndian);

    out.writeRawData("RIFF", 4);
    out << quint32(0);
    out.writeRawData("WAVE", 4);
    out.writeRawData("fmt ", 4);

    out.setByteOrder(QDataStream::LittleEndian);
    out << quint32(16);
    out << quint16(1);
    out << quint16(format.channels());
    out << quint32(format.frequency());
    out << quint32(format.frequency() * format.channels() * format.sampleSize() / 8 );
    out << quint16(format.channels() * format.sampleSize() / 8);
    out << quint16(format.sampleSize());

    out.setByteOrder(QDataStream::BigEndian);
    out.writeRawData("data", 4);
    out << quint32(0);

    Q_ASSERT(pos() == 44);
}

//Cerrar archivo WAV. Terminar cabecera.
void Audiowav::close()
{
    QDataStream out(this);

    out.setByteOrder(QDataStream::LittleEndian);
    quint32 size_file = size();
    seek(4);
    out << quint32(size_file-8);
    seek(40);
    out << quint32(size_file-44);

    QFile::close();
}
