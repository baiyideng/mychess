#include "chessboard.h"
#include <QPainter>
#include <QPen>
#include <QDebug>
#include <QPixmap>
#include <QCursor>
#include <QtWidgets/QApplication>
#include<QtWidgets/QLabel>
#include<QMouseEvent>
#include<QtWidgets/QMessageBox>
#include <QPalette>
#include <QFont>


Chessboard::Chessboard(QWidget *parent) :
    QWidget(parent)
{
    ChessPiecesList[0] = new ChessPieces(0,this);
    ChessPiecesList[1] = new ChessPieces(1,this);
    ChessPiecesList[2] = new ChessPieces(5,this);
    ChessPiecesList[3] = new ChessPieces(2,this);
    ChessPiecesList[4] = new ChessPieces(6,this);
    ChessPiecesList[5] = new ChessPieces(10,this);

    ChessPiecesList[6] = new ChessPieces(24,this);
    ChessPiecesList[7] = new ChessPieces(23,this);
    ChessPiecesList[8] = new ChessPieces(19,this);
    ChessPiecesList[9] = new ChessPieces(22,this);
    ChessPiecesList[10] = new ChessPieces(18,this);
    ChessPiecesList[11] = new ChessPieces(14,this);

    resize(400,400);

    pbStart = new QPushButton(this);
    pbStart->setText(QString::fromUtf8("开局"));
    pbStart->setGeometry(300,50,50,25);

    pbRevoke = new QPushButton(this);
    pbRevoke->setText(QString::fromUtf8("撤销"));
    pbRevoke->setGeometry(300,100,50,25);
    pbRevoke->setDisabled(true);

    pbReset = new QPushButton(this);
    pbReset->setText(QString::fromUtf8("新一局"));
    pbReset->setGeometry(300,150,50,25);

    pbAI = new QPushButton(this);
    pbAI->setText(QString::fromUtf8("行棋"));
    pbAI->setGeometry(150,300,50,25);
    pbAI->setEnabled(false);

    peInputNumber = new QLineEdit(this);
    peInputNumber->setGeometry(50,300,50,25);

    isLayout = true;
    isChoseSrc = true;
    whoseTurn = OUT_OF_PLAYER;

    connect(peInputNumber,SIGNAL(textChanged(QString)),this,SLOT(peInputNumber_text_changed(QString)));
    connect(pbStart,SIGNAL(clicked()),this,SLOT(pbStart_on_clicked()));
    connect(pbRevoke,SIGNAL(clicked()),this,SLOT(pbRevoke_on_clicked()));
    connect(pbReset,SIGNAL(clicked()),this,SLOT(pbReset_on_clicked()));
    connect(pbAI,SIGNAL(clicked()),this,SLOT(pbAI_on_clicked()));
}

Chessboard::~Chessboard()
{
}


void Chessboard::paintEvent(QPaintEvent *)
{
    QPainter painter(this);             // 创建QPainter一个对象
    QPen pen;
    pen.setColor(Qt::black);           // 设置画笔为黑色，painter一定要在paintEvent中才可以执行，不需被调用
    painter.setPen(pen);
    // 绘制棋盘
    painter.drawLine(0, 0, 0, 250);
    painter.drawLine(50, 0, 50, 250);
    painter.drawLine(100, 0, 100, 250);
    painter.drawLine(150, 0, 150, 250);
    painter.drawLine(200, 0, 200, 250);
    painter.drawLine(250, 0, 250, 250);
    painter.drawLine(0, 0, 250, 0);
    painter.drawLine(0, 50, 250, 50);
    painter.drawLine(0, 100, 250, 100);
    painter.drawLine(0, 150, 250, 150);
    painter.drawLine(0, 200, 250, 200);
    painter.drawLine(0, 250, 250, 250);

    QPalette pal;
    pal.setColor(QPalette::Background, Qt::red);

    for(int i = 0;i<12;i++)
    {      
            int nx = number_to_x(ChessPiecesList[i]);
            int ny = number_to_y(ChessPiecesList[i]);

            if(ChessPiecesList[i]->isChosed)
                ChessPiecesList[i]->pLabel->setGeometry(nx,ny,40,40);
            else
                ChessPiecesList[i]->pLabel->setGeometry(nx,ny,49,49);

            if(i < 6)
            {
                ChessPiecesList[i]->pLabel->setStyleSheet("background:blue;");
                ChessPiecesList[i]->pLabel->setText(QString::number(i+1));
            }
            else if (i > 5)
            {
                ChessPiecesList[i]->pLabel->setStyleSheet("background:red;");
                ChessPiecesList[i]->pLabel->setText(QString::number(i-5));
            }

            ChessPiecesList[i]->pLabel->setAlignment(Qt::AlignCenter);
            ChessPiecesList[i]->pLabel->setFont(QFont("",20));

            if(ChessPiecesList[i]->number_of_chessboard == OUT_OF_CHESSBOARD)
               ChessPiecesList[i]->pLabel->setVisible(false);
            else
                ChessPiecesList[i]->pLabel->setVisible(true);

    }
}

void Chessboard::mousePressEvent(QMouseEvent *m)
{
    //QCursor my(QPixmap("picture/mouse.PNG")); //为鼠标指针选择图片
    //QApplication::setOverrideCursor(my); //将鼠标指针更改为自己设置的图片
    setMouseTracking(true);//不跟踪鼠标，点击才计算

    static  int this_number = OUT_OF_CHESSBOARD;

    if(m->button() == Qt::LeftButton)//鼠标点击函数!!!!外层可嵌套一个if循环bool判断敌走还是我走，敌走调用智能下棋函数
    {
        if(m->pos().x()<=250&&m->pos().y()<=250)  //选择棋盘的点不响应
        {
            this_number = x_y_to_number(m->pos().x(),m->pos().y());

            //选择源棋子
            if(isChoseSrc)
            {
                if(__hasChessPieces(this_number,src_NumberOfChessPieces))
                {
                    if(!isLayout && (!__isMyTurn())) return;
                    isChoseSrc = false;
                    ChessPiecesList[src_NumberOfChessPieces]->isChosed = true;
                }
            }
            //选择目标位置
            else
            {
                if(isLayout)
                {
                    int des_chessPieces_number = OUT_OF_CHESSBOARD;
                    if(__hasChessPieces(this_number,des_chessPieces_number) && \
                            __isHomochromy(src_NumberOfChessPieces,des_chessPieces_number))
                        moveChessPieces_Layout(des_chessPieces_number);
                }
                else
                {
                    moveChessPieces(this_number);

                    laws.updatePuzzles(ChessPiecesList);
                    int winer;
                    if(laws.isWin(winer))
                    {
                        if(winer == RED)
                            QMessageBox::information(this,"win","红方获胜",QMessageBox::Ok);
                        else if(winer == BLUE)
                            QMessageBox::information(this,"win","蓝方获胜",QMessageBox::Ok);
                    }
                }
            }
            this->repaint();  //调用paintvent函数绘图
        }
    }

}

void Chessboard::moveChessPieces( int des_chessboard_number)
{
    __saveInstantane();
    laws.updatePuzzles(ChessPiecesList);

    if(laws.isLegitimacy(ChessPiecesList[src_NumberOfChessPieces]->number_of_chessboard,des_chessboard_number))
    {
        __saveInstantane();
        int des_chessPieces_number = OUT_OF_CHESSPIECES;
        if(__hasChessPieces(des_chessboard_number, des_chessPieces_number))
        {
            ChessPiecesList[des_chessPieces_number]->number_of_chessboard = OUT_OF_CHESSBOARD;
        }

        ChessPiecesList[src_NumberOfChessPieces]->number_of_chessboard = des_chessboard_number;
        if(whoseTurn == RED)
            whoseTurn = BLUE;
        else if(whoseTurn == BLUE)
            whoseTurn = RED;
    }

    if(src_NumberOfChessPieces != OUT_OF_CHESSPIECES)
        ChessPiecesList[src_NumberOfChessPieces]->isChosed = false;

    src_NumberOfChessPieces = OUT_OF_CHESSPIECES;
    isChoseSrc = true;
}


void Chessboard::moveChessPieces_Layout(int des_chessPieces_number)
{
    int tmp = ChessPiecesList[des_chessPieces_number]->number_of_chessboard;
    ChessPiecesList[des_chessPieces_number]->number_of_chessboard = ChessPiecesList[src_NumberOfChessPieces]->number_of_chessboard;
    ChessPiecesList[src_NumberOfChessPieces]->number_of_chessboard = tmp;

    isChoseSrc = true;
    ChessPiecesList[src_NumberOfChessPieces]->isChosed = false;
}

void Chessboard::pbStart_on_clicked()
{
    pbStart->setDisabled(true);
    isLayout = false;
    if(src_NumberOfChessPieces != OUT_OF_CHESSPIECES)
    {
        ChessPiecesList[src_NumberOfChessPieces]->isChosed = false;
        src_NumberOfChessPieces = OUT_OF_CHESSPIECES;
        isChoseSrc = true;
    }
    __saveInstantane();
    pbRevoke->setEnabled(true);
}

void Chessboard::pbRevoke_on_clicked()
{
    if(src_NumberOfChessPieces != OUT_OF_CHESSPIECES)
        ChessPiecesList[src_NumberOfChessPieces]->isChosed = false;
    src_NumberOfChessPieces = OUT_OF_CHESSPIECES;
    isChoseSrc = true;

    __recoverFromInstantane();
}

void Chessboard::pbReset_on_clicked()
{
    pbStart->setEnabled(true);
    isLayout = true;
    isChoseSrc = true;
    whoseTurn = OUT_OF_PLAYER;


    ChessPiecesList[0]->number_of_chessboard = 0;
    ChessPiecesList[1]->number_of_chessboard = 1;
    ChessPiecesList[2]->number_of_chessboard = 5;
    ChessPiecesList[3]->number_of_chessboard = 2;
    ChessPiecesList[4]->number_of_chessboard = 6;
    ChessPiecesList[5]->number_of_chessboard = 10;

    ChessPiecesList[6]->number_of_chessboard = 24;
    ChessPiecesList[7]->number_of_chessboard = 23;
    ChessPiecesList[8]->number_of_chessboard = 19;
    ChessPiecesList[9]->number_of_chessboard = 22;
    ChessPiecesList[10]->number_of_chessboard = 18;
    ChessPiecesList[11]->number_of_chessboard = 14;
    for(int i = 0; i < 12; i++) ChessPiecesList[i]->isChosed = false;

    this->repaint();
}

void Chessboard::pbAI_on_clicked()
{
    int number = peInputNumber->text().toInt();
    peInputNumber_text_changed(0);
}

void Chessboard::peInputNumber_text_changed(QString str)
{
    bool ok;
    str=str.left(1);
    int num=str.toInt(&ok);
    if(!ok||(num<1||num>6))
    {
        peInputNumber->clear();
        pbAI->setEnabled(false);
    }
    else
    {
        peInputNumber->setText(str);
        pbAI->setEnabled(true);
    }
}


bool Chessboard::__isHomochromy(int src_chessPieces_number, int des_chessPieces_number)
{
    if((0 <= src_chessPieces_number && src_chessPieces_number < 6 && \
        0 <= des_chessPieces_number && des_chessPieces_number <6) \
            || \
            (6 <= src_chessPieces_number && src_chessPieces_number < 12 && \
             6 <= des_chessPieces_number && des_chessPieces_number < 12) )
        return true;
    else return false;
}

bool Chessboard::__hasChessPieces(int chessboard_number, int &ret_chessPieces_number)
{
    for(int i = 0; i < 12; i++)
    {
        if(ChessPiecesList[i]->number_of_chessboard == chessboard_number)
        {
            ret_chessPieces_number = i;
            return true;
        }
    }
    return false;
}

bool Chessboard::__isMyTurn()
{
    if(whoseTurn == OUT_OF_PLAYER)    //第一次行棋时未规定谁先手，可任意。
    {
        //whoseTurn设为当前行棋方是因为whoseTurn的改变由moveChessPieces()函数完成。
        if(0 <= src_NumberOfChessPieces && src_NumberOfChessPieces < 6)
            whoseTurn = RED;
        else if(6 <= src_NumberOfChessPieces && src_NumberOfChessPieces <12)
            whoseTurn = BLUE;
        else return false;

        return true;
    }
    else if(whoseTurn == RED && 0 <= src_NumberOfChessPieces && src_NumberOfChessPieces < 6)
        return true;
    else if (whoseTurn == BLUE && 6 <= src_NumberOfChessPieces && src_NumberOfChessPieces <12)
        return true;
    else return false;
}

int Chessboard::x_y_to_number(int x, int y)
{
    int number = OUT_OF_CHESSBOARD;
    if(0 <= x && x < 250)
    {
        if(0 <= y && y <250)
            number = 5*(y/50) + x/50;
    }

    return number;
}

void Chessboard::__saveInstantane()
{
    for(int i = 0; i < 12; i++)
        lastStatus[i] = ChessPiecesList[i]->number_of_chessboard;
}

void Chessboard::__recoverFromInstantane()
{
    for(int i = 0; i < 12; i++)
        ChessPiecesList[i]->number_of_chessboard = lastStatus[i];

    if(whoseTurn == RED)
        whoseTurn = BLUE;
    else if(whoseTurn == BLUE)
        whoseTurn = RED;

    this->repaint();
}


