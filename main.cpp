//#include "mychess.h"
#include"chessboard.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Chessboard w;
    w.show();

    return a.exec();
}
