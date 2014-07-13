#include "log.h"
#include <QDateTime>
#include <QString>

Log::Log()
{
    QStrinf str = QDateTime::toString(Qt::ISODate)+".log";
    logFile = new QFile(str);
    logFile->open(QFile::WriteOnly | QFile::Text);
    out = new QTextStream(logFile);
}

void Log::insertTitle(int type)
{
    switch(type)
    {
    case START:
        out << "#################"+QDateTime::toString(Qt::ISODate)+"---START-----###########\n";
        break;
    case END:
        out << "#################"+QDateTime::toString(Qt::ISODate)+"---END-----###########\n";
        break;
    case RESET:
        out << "#################"+QDateTime::toString(Qt::ISODate)+"---RESET-----###########\n";
        break;
    }
}
