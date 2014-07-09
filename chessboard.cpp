#include "chessboard.h"
#include <QPainter>
#include <QPen>
#include <QDebug>
#include <QPixmap>
#include <QCursor>
#include <QtWidgets/QApplication>
#include<QtWidgets/QLabel>
#include<QMouseEvent>

Chessboard::Chessboard(QWidget *parent) :
    QWidget(parent)
{
    ChessPiecesList[0] = new ChessPieces(0,"picture/rpix1.png");
    ChessPiecesList[1] = new ChessPieces(1,"picture/rpix2.png");
    ChessPiecesList[2] = new ChessPieces(5,"picture/rpix3.png");
    ChessPiecesList[3] = new ChessPieces(2,"picture/rpix4.png");
    ChessPiecesList[4] = new ChessPieces(6,"picture/rpix5.png");
    ChessPiecesList[5] = new ChessPieces(10,"picture/rpix6.png");

    ChessPiecesList[6] = new ChessPieces(24,"picture/bpix1.png");
    ChessPiecesList[7] = new ChessPieces(23,"picture/bpix2.png");
    ChessPiecesList[8] = new ChessPieces(19,"picture/bpix3.png");
    ChessPiecesList[9] = new ChessPieces(22,"picture/bpix4.png");
    ChessPiecesList[10] = new ChessPieces(18,"picture/bpix5.png");
    ChessPiecesList[11] = new ChessPieces(14,"picture/bpix6.png");


    resize(400,400);
}

void Chessboard::paintEvent(QPaintEvent *)
{
    // 画一条直线
    QPainter painter(this);             // 创建QPainter一个对象
    QPen pen;
    pen.setColor(Qt::black);           // 设置画笔为黑色，painter一定要在paintEvent中才可以执行，不需被调用
    painter.setPen(pen);
    // 设置画笔
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

    for(int i = 0;i<12;i++)
    {
        if(0 <= ChessPiecesList[i]->number_of_location && ChessPiecesList[i]->number_of_location <25)
        {
            int nx = number_to_x(ChessPiecesList[i]);
            int ny = number_to_y(ChessPiecesList[i]);

            if(nx>=200)  //解决棋子与网格边界问题
            {
                nx = 200;
            }
            if(ny>=200)
            {
                ny = 200;
            }
            painter.drawPixmap(nx,ny,49,49,ChessPiecesList[i]->pix);

        }
    }
}

void Chessboard::mousePressEvent(QMouseEvent *m)
{
    //QCursor my(QPixmap("picture/mouse.PNG")); //为鼠标指针选择图片
    //QApplication::setOverrideCursor(my); //将鼠标指针更改为自己设置的图片
    setMouseTracking(true);//不跟踪鼠标，点击才计算

    static bool flag_chose_src = true;
   static  int this_number = -1;

    if(m->button() == Qt::LeftButton)//鼠标点击函数!!!!外层可嵌套一个if循环bool判断敌走还是我走，敌走调用智能下棋函数
    {
        if(m->pos().x()<=250&&m->pos().y()<=250)  //选择棋盘的点不响应
        {
            this_number = x_y_to_number(m->pos().x(),m->pos().y());

            //选择拟走动源棋子
            if(flag_chose_src)
            {
                for(int i = 0; i < 12; i++)
                {
                    if(ChessPiecesList[i]->number_of_location == this_number)
                    {
                        src_number_of_ChessPieces = i;
                        flag_chose_src = false;
                        break;
                    }
                }
                qDebug()<<"flag_chose_src = "<<flag_chose_src<< "     src_number_of_ChessPieces = "<<src_number_of_ChessPieces;
            }
            //选择目标位置
            else
            {
                laws.resetPuzzles();
                for(int i = 0; i < 12; i++) laws.updatePuzzles(ChessPiecesList[i]->number_of_location, i);

                if(laws.isLegitimacy(ChessPiecesList[src_number_of_ChessPieces]->number_of_location,this_number))
                {
                 //是否发生吃子
                    for(int i = 0; i < 12; i++)
                    {
                        if(ChessPiecesList[i]->number_of_location == this_number)
                        {
                            ChessPiecesList[i]->number_of_location = -1;
                            break;
                        }
                    }
                    //移动源棋子到目标位置
                    ChessPiecesList[src_number_of_ChessPieces]->number_of_location = this_number;
                     flag_chose_src = true;

                    laws.resetPuzzles();
                    for(int i = 0; i < 12; i++) laws.updatePuzzles(ChessPiecesList[i]->number_of_location, i);
                    int winer;
                    if(laws.isWin(winer)){}

                    this->repaint();  //调用paintvent函数绘图
                }
            }
        }
    }

}

int Chessboard::x_y_to_number(int x, int y)
{
    int number = -1;
    if(0 <= x && x < 250)
    {
        if(0 <= y && y <250)
            number = 5*(y/50) + x/50;
    }

    return number;
}


