/******************************************************************************
 * STR2 Content Broadcast Server
 *
 * Developed by Oscar Lopes as the final project of the Multimedia Technologies
 * Master, and sponsored by CAIAC (caiac.uab.cat)
 *
 * (C)2011 [Universidad Autònoma de Barcelona]
 ******************************************************************************/

#ifndef SUBTITLELINEDATA_H
#define SUBTITLELINEDATA_H

#include <QString>

struct SubtitleLineData {
    QString *index;
    QString *initTime;
    QString *endTime;
    QString *pageNumber;
    QString *actorName;
    QString *originalText;
    QString *translatedSubtitle;
    QString *audioDesc;
    QString *comment;
    QString *subtitleSoundFile;
    QString *audioDescSoundFile;
    QString *xmlData;
};

#endif // SUBTITLELINEDATA_H
