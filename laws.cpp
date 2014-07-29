#include "laws.h"

#include "chessboard.h"

Laws::Laws()
{
//    puzzles[25] = {NULLPIECES};
}

bool Laws::isLegitimacy(int src, int des)
{
    //红方回合
    if(0 <= puzzles[src] && puzzles[src] <= 5)
    {
        if(des == src + 1 && (src+1)%5 != 0)
            return true;
        else if(des == src + 5 && src < 20)
            return true;
        else if(des == src + 6 && src < 20 && (src+1)%5 != 0)
            return true;
        else return false;
    }
    //蓝方回合
    else if(6 <= puzzles[src] && puzzles[src] <= 11)
    {
          if(des == src - 1 && src%5 != 0)
            return true;
        else if(des == src - 5 && src > 4)
            return true;
        else if(des == src - 6 && src > 4 && src%5 != 0)
            return true;
        else return false;
    }
    //非法
    else return false;
}

bool Laws::isWin(int &winer)
{
    if(6 <= puzzles[0] && puzzles[0] <12)
    {
        winer = Chessboard::BLUE;
        return true;
    }
    else if(0 <= puzzles[24] && puzzles[24] <6)
    {
        winer = Chessboard::RED;
        return true;
    }
    else
    {
        bool redWin = true;
        int i = 24;
        while(redWin && i > 0)
        {
            if( 6 <= puzzles[i] && puzzles[i] < 12)
                redWin = false;
            i--;
        }
        if(redWin)
        {
            winer = Chessboard::RED;
            return true;
        }

        bool blueWin = true;
        int j = 0;
        while(blueWin && j < 25)
        {
            if( 0 <= puzzles[j] && puzzles[j] < 6)
                blueWin = false;
            j++;
        }
        if(blueWin)
        {
            winer = Chessboard::BLUE;
            return true;
        }

        return false;
    }
}

void Laws::updatePuzzles(ChessPieces **chessPiecesList)
{
    for(int i = 0; i < 25; i++) puzzles[i] = NULLPIECES ;
    for(int i = 0; i < 12; i++)
    {
        if(chessPiecesList[i]->number_of_chessboard != Chessboard::OUT_OF_CHESSBOARD)
            puzzles[chessPiecesList[i]->number_of_chessboard] = i;
        this->chessPiecesList[i] = chessPiecesList[i]->number_of_chessboard;
    }
}



