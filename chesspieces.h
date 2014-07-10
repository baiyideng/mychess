#ifndef CHESSPIECES_H
#define CHESSPIECES_H

#include <QPixmap>

class ChessPieces
{
public:
    ChessPieces(int number, QString str_Location_Picture)
    {
        number_of_chessboard = number;
        pix.load(str_Location_Picture);
    }

    int number_of_chessboard;
    QPixmap pix;
};

#endif // CHESSPIECES_H
