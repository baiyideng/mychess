#ifndef LAWS_H
#define LAWS_H

#include"chesspieces.h"

class Laws
{
public:
    Laws();
    bool isLegitimacy(int src,int des);
    bool isWin(int &winer);
    void updatePuzzles(ChessPieces ** chessPiecesList);
private:
    enum PiecesStatus
    {
        NULLPIECES = 12
    };
    int puzzles[25];          //索引号：对应棋盘位置编号。 内容：0-11代表相应棋子；12代表空。
    int chessPiecesList[12];  //索引号：棋子编号。 内容：所在棋盘位置编号。
};

#endif // LAWS_H
