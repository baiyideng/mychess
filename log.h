#ifndef LOG_H
#define LOG_H

#include<deque>
#include <QFile>
#include <QTextStream>

#define MAX_COUNT 20

class OneStep
{
    explicit OneStep(int srcPieces, int srcChessboard, int des)\
        :srcNumber_ChessPieces(srcPieces),\
          srcNumber_Chessboard(srcChessboard),\
          desNumber_Chessboard(des){}

    int srcNumber_ChessPieces;
    int srcNumber_Chessboard;

    int desNumber_Chessboard;
};

class Log
{
public:
    Log();
    void insertTitle(int type);

    enum TitleType
    {
        START,
        END,
        RESET
    };
private:
  int count;
  std::deque<OneStep> log_stack;

  QFile* logFile;
  QTextStream* out;
};

#endif // LOG_H
