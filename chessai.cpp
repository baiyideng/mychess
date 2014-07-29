
#include "chessai.h"
#include <string.h>
#include <qmath.h>


ChessAI::ChessAI(bool isRed):isRed(isRed)//,Can_Not_Kill_All(false)
{
    redcantkillall=false;
    bluecantkillall=false;
}

void ChessAI::slotAI(int DicePoints, const int *lpPiecesPosition)
{
    for(int i=0;i<6;i++)
    {
        EnableSteps[i].PiecesNo=-1; //清空下一步可能的走法
        EnableSteps[i].NextPosition=-1;
        EnableSteps[i].Score=0;
    }
    //建立一个记录各子位置的棋盘，方便计算
    for(int i=0;i<25;i++)
    {
        ChessBoad[i]=-1;//无子时为-1
    }
    for(int i=0;i<12;i++)
    {
        if(lpPiecesPosition[i]!=-1)
        {
            ChessBoad[lpPiecesPosition[i]]=i;
        }
    }

    //统计棋子数
    RedPiecesCount=0;
    BluePiecesCount=0;
    for(int i=0;i<12;i++)
    {
        if(lpPiecesPosition[i]!=-1)
        {
            if(i<6) RedPiecesCount++;
            else    BluePiecesCount++;
        }
    }

    redflxcnt=0;
    blueflxcnt=0;
    //计算各自的灵活度
    for(int i=0;i<6;i++)
    {
        getFlexibility(lpPiecesPosition,redflx[i],i);
        redflxcnt+=redflx[i].fb;
        getFlexibility((lpPiecesPosition+6),blueflx[i],i);
        blueflxcnt+=blueflx[i].fb;
    }

    //计算各子离终点的距离
    for(int i=0;i<6;i++)
    {
        reddistance[i] = getDistance(lpPiecesPosition[i],24);
        bluedistance[i]= getDistance(lpPiecesPosition[6+i],0);
    }

    //得到各方离终点最近的棋子
    farestblue =0;
    farestred=0;
    for(int i=1;i<6;i++)
    {
        if(bluedistance[farestblue]>bluedistance[i])
        {
            farestblue=i;
        }

        if(reddistance[farestred]>reddistance[i])
        {
            farestred=i;
        }
    }

    if(isRed) this->RedAI(DicePoints,lpPiecesPosition);
    else       this->BlueAI(DicePoints,lpPiecesPosition);
}

void ChessAI::RedAI(int DicePoints, const int *lpPiecesPosition)
{
    //此点数所能走的棋
    DicePoints--;
    if(lpPiecesPosition[DicePoints]!=-1)//此子未被吃
    {
        for(int i=0;i<3;i++)
        {
            EnableSteps[i].PiecesNo=DicePoints;
        }
    }
    else
    {//此子已被吃
        int stepno=0;
        int k=2;
        int pno;
        while(k--)
        {
            pno=DicePoints;
            if(k==1)
                while((++pno < 6) && lpPiecesPosition[pno] == -1 );
            else
                while((--pno > 0) && lpPiecesPosition[pno] == -1);

            if(pno>=0 && pno < 6)
            {
                for(int i=0;i<3;i++)
                {
                    EnableSteps[stepno++].PiecesNo = pno;
                }
            }
        }
    }

    int stepcount=0;//可走的总步数
    //得出可能走的位置
    for(int i=0;i<=3;i+=3)
    {
        // memcpy(EnableSteps[i].positons,lpPiecesPosition,48);
        if(X(lpPiecesPosition[EnableSteps[i].PiecesNo]) < 4)
        {//横走
            EnableSteps[i].NextPosition = lpPiecesPosition[EnableSteps[i].PiecesNo]+1;
            stepcount++;
        }
        else
        {
            EnableSteps[i].NextPosition = -1;
        }
        if(lpPiecesPosition[EnableSteps[i+1].PiecesNo]  < 20)
        {//竖走
            EnableSteps[i+1].NextPosition = lpPiecesPosition[EnableSteps[i+1].PiecesNo]+5;
            stepcount++;
        }
        else
        {
            EnableSteps[i+1].NextPosition = -1;
        }
        if((EnableSteps[i].NextPosition != -1)&&(EnableSteps[i+1].NextPosition != -1))
        {//斜走
            EnableSteps[i+2].NextPosition = lpPiecesPosition[EnableSteps[i+2].PiecesNo]+6;
            stepcount++;
        }
        else
        {
            EnableSteps[i+2].NextPosition = -1;
        }
        if(EnableSteps[3].PiecesNo == -1) break;
    }

    //判断是否失去全歼能力
    if(!redcantkillall)
    {
        redcantkillall=RedCanNotKillAll(lpPiecesPosition,freedomblue);
    }
    if(!bluecantkillall)
    {
        bluecantkillall=BlueCanNotKillAll(lpPiecesPosition,freedomred);
    }

    //评分
    if(stepcount>1)
    {
        int positions[12];//用于保存此步走后的棋子位置
        int KillNo=-1;//被吃子号
        int Situation;
        if((!redcantkillall)&&(!bluecantkillall))
        {
            Situation=1;
        }
        else if(redcantkillall&&(!bluecantkillall))
        {
            Situation=2;
        }
        else if((!redcantkillall)&&(bluecantkillall))
        {
            Situation=3;
        }
        else
        {
            Situation=4;
        }

        int k=-1;
        for(int i=0;i<stepcount;i++)
        {
            while(EnableSteps[++k].PiecesNo==-1 || EnableSteps[k].NextPosition==-1);
            if(EnableSteps[k].NextPosition==24)
            {//此步直接走到终点
                EnableSteps[k].Score+=10000;
                break;
            }
            memcpy(positions,lpPiecesPosition,sizeof(positions));
            positions[EnableSteps[k].PiecesNo]=EnableSteps[i].NextPosition;//位置改变
            //判断是否吃子
            if( (KillNo = ChessBoad[EnableSteps[k].NextPosition]) != -1)
            {//发生吃子
                positions[KillNo]=-1;
                if(KillNo>=0 && KillNo < 6)
                {//我方子被吃
                    RedPiecesCount--;
                    Flexibility flx[6];
                    int flxcnt=0;
                    for(int i=0;i<6;i++)
                    {
                        getFlexibility(positions,flx[i],i);
                        flxcnt+=flx[i].fb;
                    }

                    switch (Situation)
                    {
                    case 1:
                    case 2:
                    {
                        if(RedPiecesCount>3)
                        {
                            if(flxcnt>redflxcnt)
                            {//吃己方子 灵活度增加 加分
                                EnableSteps[k].Score += 30*(flxcnt-redflxcnt+reddistance[KillNo]);//10倍灵活度改变量+被吃子距离
                            }
                        }
                        else
                        {//己方子不大于3 扣分
                            EnableSteps[k].Score -= 10000*qPow(double(redflx[KillNo].fb)/6.0,reddistance[KillNo]);
                        }
                        break;
                    }
                    case 3:
                    case 4:
                    {//由于对方失去全歼能力 吃己方子增加灵活度
                        if(KillNo!=farestred)
                        {
                            EnableSteps[k].Score += 10000*qPow(double(flx[farestred].fb)/6.0,reddistance[farestred]);
                        }
                        else
                        {
                            EnableSteps[k].Score += 10000* qPow(double(flx[EnableSteps[k].PiecesNo].fb)/6.0,reddistance[EnableSteps[k].PiecesNo]);
                        }
                        break;
                    }
                    default:
                        break;
                    }
                }
                else
                {//敌方子被吃
                    KillNo-=6;
                    if(bluedistance[KillNo]==1)
                    {//被吃子离我方终点只有一步
                        EnableSteps[k].Score+=5000;
                        continue;
                    }
                    Flexibility flx[6];
                    int flxcnt=0;
                    for(int i=0;i<6;i++)
                    {
                        getFlexibility((positions+6),flx[i],i);
                        flxcnt+=flx[i].fb;
                    }
                    switch (Situation) {
                    case 1:
                    case 3:
                    {//能全歼敌方的情况下
                        EnableSteps[k].Score += 30*(blueflxcnt-flxcnt+4-BluePiecesCount);
                        EnableSteps[k].Score += 10000*qPow(double(blueflx[KillNo].fb)/6.0,bluedistance[KillNo]);
                        //EnableSteps[k].Score -=  1000*qo(flx[farestblue].fb-blueflx[farestblue].fb)/bluedistance[farestblue];
                        if(KillNo!=farestblue)
                        {//吃掉子会增加对方最远子的灵活度
                            EnableSteps[k].Score -= 5000*qPow(double(flx[farestblue].fb-blueflx[farestblue].fb)/6.0,bluedistance[farestblue]);
                        }
                        break;
                    }
                    case 2:
                    case 4:
                    {//失去全歼能力的情况下，扣分
                        EnableSteps[k].Score -= 10000*qPow( double(flx[freedomblue].fb)/6.0,bluedistance[freedomblue]);
                        break;
                    }
                    default:
                        break;
                    }
                }
            }

            //判断走完该步后的局面
            //计算被吃的可能
            Flexibility enemyflx[6];
            for(int i=0;i<6;i++)
            {
                getFlexibility((positions+6),enemyflx[i],i);
            }
            Flexibility killedflx;
            bool bv=X(positions[EnableSteps[k].NextPosition]) < 4;
            bool bh=positions[EnableSteps[k].NextPosition] < 20;

            int blueno;
            if(bv)
            {//横向
                blueno=ChessBoad[EnableSteps[k].NextPosition + 1];
                if(blueno>6&&blueno<12)
                {
                    blueno-=6;
                    killedflx+=enemyflx[blueno];
                }
            }
            if(bh)
            {//竖向
                blueno=ChessBoad[EnableSteps[k].NextPosition + 5];
                if(blueno>6&&blueno<12)
                {
                    blueno-=6;
                    killedflx+=enemyflx[blueno];
                }
            }
            if(bv&&bh)
            {//斜向
                blueno=ChessBoad[EnableSteps[k].NextPosition + 6];
                if(blueno>6&&blueno<12)
                {
                    blueno-=6;
                    killedflx+=enemyflx[blueno];
                }
            }

            Flexibility ourflx[6];
            int ourflxcnt=0;
            for(int i=0;i<6;i++)
            {
                getFlexibility(positions,ourflx[i],i);
                ourflxcnt+=ourflx[i].fb;
            }

            if(killedflx.fb!=0)
            {
                int nowpositions[6];
                memcpy(nowpositions,positions,sizeof(nowpositions));
                nowpositions[EnableSteps[k].PiecesNo]=-1;
                Flexibility nowflx[6];
                int nowflxcnt=0;
                for(int i=0;i<6;i++)
                {
                    getFlexibility(nowpositions,nowflx[i],i);
                    nowflxcnt+=nowflx[i].fb;
                }

                switch (Situation) {
                case 1:
                case 2:
                {
                    if(RedPiecesCount > 3)
                    {
                        if(nowflxcnt > ourflxcnt)
                        {//被吃后灵活度若能增加
                            EnableSteps[k].Score += 100* killedflx.fb;
                        }
                        if(EnableSteps[k].PiecesNo!=farestred && nowflx[farestred].fb > ourflx[farestred].fb)
                        {//最远子灵活度增加
                            EnableSteps[k].Score +=100* killedflx.fb;
                        }
                    }
                    else
                    {
                        EnableSteps[k].Score -=200 * killedflx.fb;
                    }
                    break;
                }
                case 3:
                case 4:
                {//对手已失去全歼能力
                    EnableSteps[k].Score += 10000*qPow( double(redflx[farestred].fb)/6.0 ,reddistance[farestred]);
                    break;
                }
                default:
                    break;
                }
            }

            //判断此步对全歼能力的影响
            bool ourcantkillall=redcantkillall;
            bool enemycantkillall=bluecantkillall;
            int cantkillblueno=-1;
            int cantkillredno=-1;
            if(!ourcantkillall)
            {
                ourcantkillall = RedCanNotKillAll(positions,cantkillblueno);
            }
            if(!enemycantkillall)
            {
                enemycantkillall=BlueCanNotKillAll(positions,cantkillredno);
            }
            switch (Situation) {
            case 1:
            {
                if(ourcantkillall && enemycantkillall)
                {
                    EnableSteps[k].Score +=20000* (qPow(double(redflx[cantkillredno].fb)/6.0,reddistance[cantkillredno])
                                                   -qPow(double(blueflx[cantkillblueno].fb)/6.0,bluedistance[cantkillblueno]));
                }
                break;
            }
            case 2:
            {
                EnableSteps[k].Score +=20000* (qPow(double(redflx[cantkillredno].fb)/6.0,reddistance[cantkillredno]));
                break;
            }
            case 3:
            {
                EnableSteps[k].Score -=20000* (qPow(double(redflx[cantkillblueno].fb)/6.0,reddistance[cantkillblueno]));
                break;
            }
            case 4:
            {
                break;
            }
            default:
                break;
            }
            EnableSteps[k].Score+=10000*qPow(double(redflx[EnableSteps[k].PiecesNo].fb)/6.0,getDistance(EnableSteps[k].NextPosition,24));
        }
    }
    else
    {
        int n=-1;
        while(EnableSteps[++n].PiecesNo==-1||EnableSteps[n].NextPosition == -1);
        emit signalAI(EnableSteps[n].PiecesNo,EnableSteps[n].NextPosition);
        return;
    }


    int n=-1;
    while(EnableSteps[++n].PiecesNo==-1||EnableSteps[n].NextPosition == -1);
    for(int i=n+1;i<6;i++)
    {
        if(EnableSteps[i].PiecesNo!=-1&&EnableSteps[i].NextPosition!=-1)
        {
            if(EnableSteps[i].Score>EnableSteps[n].Score)
            {
                n=i;
            }
        }
    }
    emit signalAI(EnableSteps[n].PiecesNo,EnableSteps[n].NextPosition);
    return;
}


void ChessAI::BlueAI(int DicePoints, const int *lpPiecesPosition)
{
}

void ChessAI::getFlexibility(const int *lpPiecesPositions, Flexibility &flexibility, int i)
{//计算所某子灵活度                          六子位置的数组             用于返回灵活度   编号 0-5
    if( i<0 || i>5 || lpPiecesPositions[i]==-1)
    {
        flexibility.fb=0;
        flexibility.ac=0;
    }
    else
    {
        flexibility.fb=1;
        //计算辅助元素
        flexibility.ac=1<<i;
        //当两边有子被吃时，继续计算灵活度
        int j=i;
        while(--j >= 0 && lpPiecesPositions[j]==-1)//向下搜索
        {
            flexibility.fb++;
            flexibility.ac |= 1<< j;
        }
        while(++i < 6 && lpPiecesPositions[i]==-1) //向上搜索
        {
            flexibility.fb++;
            flexibility.ac |= 1 << i;
        }
    }
}

int ChessAI::getDistance(const int p1, const int p2)
{//计算两位置间距离
    if(p1==-1 || p2 ==-1) return 6;
    int dx=X(p1)>X(p2)? X(p1)-X(p2):X(p2)-X(p1);
    int dy=Y(p1)>Y(p2)?  Y(p1)-Y(p2):Y(p2)-Y(p1);

    return (dx>dy? dx:dy);
}

bool ChessAI::RedCanNotKillAll(const int *lpPiecesPosition ,int &BlueNo)
{//判断红方是否失去全歼能力
    BlueNo=-1;
    for(int i=6,j;i<12;i++)
    {
        if(lpPiecesPosition[i]!=-1)
        {
            for(j=0;j<6;j++)
            {      //红方子                            蓝方子在红方子可走到的方位内
                if(lpPiecesPosition[j]!=-1 && X(lpPiecesPosition[i]) >= X(lpPiecesPosition[j]) && Y(lpPiecesPosition[i]) >= Y(lpPiecesPosition[j])) break;
            }
            if(j == 6)
            {//若均不在红方控制范围内
                BlueNo=i;
                return true;
            }
        }
    }
    return false;
}

bool ChessAI::BlueCanNotKillAll(const int *lpPiecesPosition, int &RedNo)
{ //判断蓝方是否失去全歼能力
    RedNo=-1;
    for(int i=0,j;i<6;i++)
    {
        if(lpPiecesPosition[i]!=-1)
        {
            for(j=6;j<12;j++)
            {
                if(lpPiecesPosition[j] != -1 && X(lpPiecesPosition[i]) <= X(lpPiecesPosition[j]) && Y(lpPiecesPosition[i]) <= Y(lpPiecesPosition[j])) break;
            }

            if(j == 12)
            {
                RedNo=i;
                return true;
            }
        }
    }
    return false;
}




Flexibility Flexibility::operator+(const Flexibility &f)
{//计算灵活度的和
    Flexibility rf;
    rf.fb = fb+f.fb;
    rf.ac=ac | f.ac;
    unsigned char tac=ac&f.ac;
    while (tac)
    {
        rf.fb--;
        tac=(tac>>1)&(tac<<1);
    }
    return rf;
}

Flexibility Flexibility::operator +=(const Flexibility &f)
{
    Flexibility rf=*this+f;
    this->ac=rf.ac;
    this->fb=rf.fb;
    return rf;
}
