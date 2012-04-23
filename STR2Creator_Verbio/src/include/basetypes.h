#ifndef BASETYPES_H
#define BASETYPES_H

#include "src\include\subtitles.h"
#include <QTreeWidgetItem>
#include <QMdiSubWindow>
#include <QIcon>
#include <QProcess>


#ifndef SUB_DATA_TYPE
#define SUB_DATA_TYPE

struct SubData {
    Subtitles *sub;
    QTreeWidgetItem *treeNodeItem;
    QMdiSubWindow *subWindow;
    QIcon icon;
    QString voiceName;
    bool isSupported;
};
#endif


#endif // BASETYPES_H
