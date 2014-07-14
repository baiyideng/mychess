#ifndef CHESSPIECES_H
#define CHESSPIECES_H

#include <QtWidgets/QLabel>

class ChessPieces
{
public:
    ChessPieces(int number, QWidget *parent = 0)
    {
        number_of_chessboard = number;
        pLabel = new QLabel(parent);
        isChosed = false;
    }
    virtual ~ChessPieces(){}

    int number_of_chessboard;
    QLabel *pLabel;
    bool isChosed;
};

#endif // CHESSPIECES_H
