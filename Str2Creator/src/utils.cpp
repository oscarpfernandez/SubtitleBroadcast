#include "utils.h"

/******************************************************************************
 * Description:
 * This class implements general purpose static methods
 * to be used across the project.
 ******************************************************************************/


/******************************************************************************
 * Displays a warning dialog with a custom message.
 * Args:
 *  - QWidget *parentWidget: caller widget.
 *  - QString message: custom warning message to display.
 ******************************************************************************/
void Utils::showWarningDialog(QWidget *parentWidget,
                              QString message)
{
    QMessageBox warningMessage(parentWidget);
    warningMessage.setText(message);
    warningMessage.setWindowTitle("Warning");
    warningMessage.setIcon(QMessageBox::Warning);
    warningMessage.exec();
}

/******************************************************************************
 * Compresses a directory and al the containing files & folders to a zip file.
 * This uses the system's zip utility to perform the task.
 * Args:
 *  - QString &zipApplication: application name (e.g /usr/bin/zip)
 *    Using this will give more flexibility if is required to migrate to other
 *    OS & tools
 *  - QString &zipDir: the directory that will be compressed.
 *    We won't verify if exists, if so the zip application will fail and notify
 *    this error.
 *  - QString &zipName: the full path to the zip file that will be generated.
 *    (e.g /home/user/export/project.zip)
 * Returns:
 *  - ErrorDetail object with all the info about the running process.
 ******************************************************************************/
ErrorDetail* Utils::zipDirectory(const QString &zipApplication,
                                 const QString &zipDir,
                                 const QString &zipName)
{
    ErrorDetail *errorDetail = new ErrorDetail();

    QProcess::ExitStatus exitStat = QProcess::NormalExit;

    // zip the temporary directory
    QStringList commandArgs;
    commandArgs.append("-r");
    commandArgs.append(zipName);
    commandArgs.append(".");

    QProcess *zipProc = new QProcess();
    zipProc->setWorkingDirectory(zipDir);

    zipProc->startDetached(zipApplication, commandArgs);

    zipProc->waitForStarted(3000);

    zipProc->waitForFinished(30*1000);

    if (zipProc->exitStatus() != QProcess::NormalExit )
    {
        //zip didn't exit normally
        exitStat = QProcess::CrashExit;
    }
    else
    {
        //normal exit, but verify exit code for last check...
        if (zipProc->exitStatus()>0)
        {
            //exit code is not 0. some something messy occured...
            exitStat = QProcess::CrashExit;
        }
    }


    // if everything was Ok, this must be 0.
    errorDetail->exitCode    = zipProc->exitCode();
    errorDetail->errorString = zipProc->errorString();
    return errorDetail;
}

/******************************************************************************
 * Uncompresses a zip file to a directory.
 * This uses the system's unzip utility to perform the task.
 * Args:
 *  - QString &unzipApplication: application name (e.g /usr/bin/unzip)
 *    Using this will give more flexibility if is required to migrate to other
 *    OS & tools
 *  - QString &zipName: the full path to the zip file that will be generated.
 *    (e.g /home/user/export/project.zip)
 *  - QString &toDir: the directory to where the contents will be  decompressed.
 * Returns:
 *  - ErrorDetail object with all the info about the running process.
 ******************************************************************************/
ErrorDetail* Utils::unzipDirectory(const QString &unzipApplication,
                                   const QString &zipFileName,
                                   const QString &toDir)
{
    ErrorDetail *errorDetail = new ErrorDetail();

    QProcess::ExitStatus exitStat = QProcess::NormalExit;

    // zip the temporary directory
    QStringList commandArgs;
    commandArgs.append(zipFileName);
    commandArgs.append("-d");
    commandArgs.append(toDir);

    QProcess *zipProc = new QProcess ();
    zipProc->setWorkingDirectory(toDir);

    zipProc->start(unzipApplication, commandArgs);
    zipProc->waitForFinished();

    if (zipProc->exitStatus() != QProcess::NormalExit )
    {
        //zip didn't exit normally
        exitStat = QProcess::CrashExit;
    }
    else
    {
        //normal exit, but verify exit code for last check...
        if (zipProc->exitStatus()>0)
        {
            //exit code is not 0. some something messy occured...
            exitStat = QProcess::CrashExit;
        }
    }


    // if everything was Ok, this must be 0.
    errorDetail->exitCode    = zipProc->exitCode();
    errorDetail->errorString = zipProc->errorString();
    return errorDetail;
}

/******************************************************************************
 * Creates a .ogg file from a .wav using the oggenc tool.
 * Args:
 * - const QString &fileToConvert: the .wav file name.
 * - const QString &fileToGenerate: the .ogg file name
 * - const QString &workingDirectory: the directory where all the file are stored
 ******************************************************************************/
void Utils::createOggFile(const QString &fileToConvert,
                          const QString &fileToGenerate,
                          const QString &workingDirectory)
{
    qDebug("ENTRY Utils::createOggFile");
    qDebug("\tfile name = "+fileToConvert.toAscii()+
           "\n\tworkingDir = "+workingDirectory.toAscii());


    QStringList commandArgs;
    commandArgs.append(fileToConvert);
    commandArgs.append("-o");
    commandArgs.append(fileToGenerate);

    QProcess proc;
    proc.setWorkingDirectory(workingDirectory);

    const QString appName = "oggenc";

    proc.start(appName, commandArgs);
    if(!proc.waitForStarted()){
        qDebug("Warning! Utils::createOggFile: encoder did not started check if the app is installed!");
        qDebug("EXIT Utils::createOggFile");
        return;
    }

    if(!proc.waitForFinished(5000))
    {
        qDebug("Warning! Utils::createOggFile: encoder did not finished!");
        qDebug("EXIT Utils::createOggFile");
        return;
    }
    qDebug("\t"+fileToGenerate.toAscii()+ " created successfully!!!");

    qDebug("EXIT Utils::createOggFile");
}

