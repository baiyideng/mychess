#ifndef CHESSAI_H
#define CHESSAI_H
#include <QObject>

//  x[0,4] y[0,4]
// position = x+y*5
#define X(position) (position%5)
#define Y(position) (position/5)

class Step
{
public:
    int PiecesNo;           //棋号,若为-1,表示不可走
    int NextPosition;      //下一位置
    int P_enemykill;        // 此位置被对方吃的概率
    //    int positons[12];      //下完此步后的局面
    int Score;                //对此步的评分
};

class Flexibility
{
public:
    explicit Flexibility(int fb=0,unsigned char ac=0):fb(fb),ac(ac){}
    int fb;//灵活度
    Flexibility operator+(const Flexibility &f);
    Flexibility operator +=(const Flexibility &f);
    unsigned char ac;//用于辅助计算
};

class ChessAI:public QObject
{
    Q_OBJECT
public:
    explicit ChessAI(bool isRed=true);
    ~ChessAI(){}

    //public slots:
    void slotAI(int DicePoints, const int *lpPiecesPosition);
    void RedAI(int DicePoints,const int* lpPiecesPosition);
    void BlueAI(int DicePoints,const int* lpPiecesPosition);

    static void getFlexibility(const int *lpPiecesPositions, Flexibility &flexibility, int i);//获取某子的灵活度   0<=i<6
    static int getDistance(const int p1,const int p2);//计算两点间的距离
    static  bool RedCanNotKillAll(const int *lpPiecesPosition, int &BlueNo);//判断红方失去全歼能力，参数返回无法歼杀的棋号
    static bool  BlueCanNotKillAll(const int *lpPiecesPosition,int &RedNo);//判断蓝方失去全歼能力
    signals:
    void signalAI(int,int);
private:
    bool isRed;
    Step EnableSteps[6];//下一步走法
    bool redcantkillall;
    bool bluecantkillall;
    int freedomblue;//无法消灭的敌人
    int freedomred;//我方不会被吃的棋子

    int RedPiecesCount;//我方棋子总数
    int BluePiecesCount;//敌方棋子总数

    int farestred;//最靠近我方起点的敌人编号
    int farestblue;//最靠近终点的我方棋子编号

    Flexibility redflx[6];//保存原来的灵活度
    Flexibility blueflx[6];

    int redflxcnt;
    int blueflxcnt;

    int reddistance[6];//保存各子距离终点的距离
    int bluedistance[6];

    int ChessBoad[25];//棋盘
};

#endif // CHESSAI_H
