/******************************************************************************
 * STR2 Content Broadcast Server
 *
 * Developed by Oscar Lopes as the final project of the Multimedia Technologies
 * Master, and sponsored by CAIAC (caiac.uab.cat)
 *
 * (C)2011 [Universidad Autònoma de Barcelona]
 ******************************************************************************/

#ifndef SUBTITLEDATA_H
#define SUBTITLEDATA_H

#include "subtitlelinedata.h"
#include <QMap>

struct SubtitleData {
    int currentLineIndex;
    QMap<int,SubtitleLineData*> *englishSubs;
    QMap<int,SubtitleLineData*> *spanishSubs;
    QMap<int,SubtitleLineData*> *catalanSubs;
    QMap<int,SubtitleLineData*> *italianSubs;
};

#endif // SUBTITLEDATA_H
