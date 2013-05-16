#include "assistant.h"

#include "QtGui"

Assistant::Assistant() : proc(NULL)
{
}


Assistant::~Assistant()
{
    if (proc && proc->state() == QProcess::Running)
    {
        proc->terminate();
        proc->waitForFinished(3000);
    }
    delete proc;
}

void Assistant::showDocumentation(const QString &file)
{
    if (!startAssistant())
        return;

    QByteArray ba("SetSource ");
    ba.append("qthelp://com.codesaur.simpletextviewer/doc/");

    proc->write(ba + file.toLocal8Bit() + '\n');
}

bool Assistant::startAssistant()
{
    if (!proc)
        proc = new QProcess();

    if (proc->state() != QProcess::Running) {
        QString app = QLibraryInfo::location(QLibraryInfo::BinariesPath) + QDir::separator();
#if !defined(Q_OS_MAC)
        app += QLatin1String("assistant");
#else
        app += QLatin1String("Assistant.app/Contents/MacOS/Assistant");
#endif

        QLatin1String collection("./doc/cpnsimulator.qhc");
        if(!QFile::exists(collection))
        {
            QMessageBox::critical(0, QObject::tr("CPNSimulator Help"),
                QObject::tr("Help collection file %1 not found").arg(collection));
            return false;
        }
        QStringList args;
        args << QLatin1String("-collectionFile")
            << collection
            << QLatin1String("-enableRemoteControl");
        proc->start(app, args);

        if (!proc->waitForStarted())
        {
            QMessageBox::critical(0, QObject::tr("CPNSimulator Help"),
                QObject::tr("Unable to launch Qt Assistant (%1)").arg(app));
            return false;
        }
    }
    return true;
}
