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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include "chesspieces.h"
#include"laws.h"

#include "chessai.h"

class Chessboard : public QWidget
{
    Q_OBJECT
public:
    explicit Chessboard(QWidget *parent = 0);
    virtual ~Chessboard();
    void moveChessPieces(int &src_chessboard_number, int &des_chessboard_number);
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *m);  //mousemoveevent为鼠标拖拽函数

    ChessPieces* ChessPiecesList[12];

    enum StatusFlages
    {
        OUT_OF_CHESSBOARD = -1,
        OUT_OF_CHESSPIECES = -1,
        OUT_OF_PLAYER = -1,
        RED = 5,
        BLUE = 11,
    };

signals:

public slots:
    void pbStart_on_clicked();
    void pbRevoke_on_clicked();
    void pbReset_on_clicked();
    void pbAI_on_clicked();
    void pbRand_on_clicked();
    void peInputNumber_text_changed(QString str);

    void slotAI(int src,int dest);

private:
    bool __isHomochromy(int &src_chessPieces_number,int &des_chessPieces_number) const;
    bool __hasChessPieces(int &chessboard_number , int &ret_chessPieces_number);
    bool __isMyTurn();

    void moveChessPieces_Layout(int des_chessPieces_number);

    int number_to_x(ChessPieces* cp){return 50 * (cp->number_of_chessboard % 5);}
    int number_to_y(ChessPieces* cp){ return 50 * (cp->number_of_chessboard / 5);}
    int x_y_to_number(int x, int y);    //return the number of chessboard

    void __saveInstantane();
    void __recoverFromInstantane();

    int src_NumberOfChessPieces;

    Laws laws;

    int whoseTurn;

    bool isChoseSrc;
    bool isLayout;      //正式开始前的棋子布局状态
    QPushButton* pbStart;
    QPushButton* pbRevoke;
    QPushButton* pbReset;

    int lastStatus[12];
    int lastLayout[12];

    QLineEdit* peInputNumber;
    QPushButton* pbAI;
    QPushButton* pbRand;

    ChessAI *ai;
};

#endif // CHESSBOARD_H
