#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QWidget>
#include <QMessageBox>
#include <QProcess>

struct ErrorDetail{
    QString errorString;
    int exitCode;
    QProcess::ExitStatus exitStatus;
};

class Utils
{
public:
    static void showWarningDialog(QWidget *parentWidget,
                                  QString message);
    static ErrorDetail* zipDirectory(const QString &zipApplication,
                                     const QString &zipDir,
                                     const QString &zipName);
    static ErrorDetail* unzipDirectory(const QString &unzipApplication,
                                       const QString &zipFileName,
                                       const QString &toDir);
    static void createOggFile(const QString &fileToConvert,
                              const QString &fileToGenerate,
                              const QString &workingDirectory);

};

#endif // UTILS_H
