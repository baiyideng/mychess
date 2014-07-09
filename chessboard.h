#ifndef CHESSBOARD_H
#define CHESSBOARD_H

/*###################################################################################################
 * 棋盘为5X5布局，编号如下:
 *
 * ********************
 *  0 * 1 * 2 * 3 * 4 *
 * ********************
 *  5 * 6 * 7 * 8 * 9 *
 * ********************
 * 10 *11 *12 *13 *14 *
 * ********************
 * 15 *16 *17 *18 *19 *
 * ********************
 * 20 *21 *22 *23 *24 *
 * ********************
 *
 * 红蓝双方各执6子，
 * 红方1-6子编号为0-5；
 * 蓝方1-6子编号为6-11；
 * #################################################################################################
*/

#include <QWidget>

#include "chesspieces.h"
#include"laws.h"

class Chessboard : public QWidget
{
    Q_OBJECT
public:
    explicit Chessboard(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *m);  //mousemoveevent为鼠标拖拽函数

    ChessPieces *ChessPiecesList[12];

signals:

public slots:

private:
    int number_to_x(ChessPieces* cp){return 50 * (cp->number_of_location % 5);}
    int number_to_y(ChessPieces* cp){ return 50 * (cp->number_of_location / 5);}
    int x_y_to_number(int x, int y);

    int src_number_of_ChessPieces;
    int des_number_of_Chessboard;
    Laws laws;

};

#endif // CHESSBOARD_H
