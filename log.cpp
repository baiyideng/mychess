#include "log.h"
#include <QString>

Log::Log()
{
    QString str = dateTime.toString(Qt::ISODate)+".log";
    logFile = new QFile(str);
    logFile->open(QFile::WriteOnly | QFile::Text);
    out = new QTextStream(logFile);
}

void Log::insertTitle(int type)
{
    switch(type)
    {
    case START:
        //out << "#################"+dateTime.toString(Qt::ISODate)+"---START-----###########\n";
        break;
    case END:
       // out << "#################"+dateTime.toString(Qt::ISODate)+"---END-----###########\n";
        break;
    case RESET:
       // out << "#################"+dateTime.toString(Qt::ISODate)+"---RESET-----###########\n";
        break;
    }
}
