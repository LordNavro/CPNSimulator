#ifndef ASSISTANT_H
#define ASSISTANT_H

#include <QtCore/QByteArray>
#include <QtCore/QDir>
#include <QtCore/QLibraryInfo>
#include <QtCore/QProcess>

#include <QtGui/QMessageBox>

class Assistant
{
public:
    Assistant();
    ~Assistant();
    void showDocumentation(const QString &file = "index.html");
private:
    bool startAssistant();
    QProcess *proc;
};

#endif // ASSISTANT_H
