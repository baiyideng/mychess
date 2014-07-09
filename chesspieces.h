#ifndef CHESSPIECES_H
#define CHESSPIECES_H

#include <QPixmap>

class ChessPieces
{
public:
    ChessPieces(int number, QString str_Location_Picture)
    {
        number_of_location = number;
        pix.load(str_Location_Picture);
    }

    int number_of_location;
    QPixmap pix;
};

#endif // CHESSPIECES_H
