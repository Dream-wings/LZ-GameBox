#include "chessendscene.h"
#include "wuziqi.h"
#include <QPainter>
ChessEndScene::ChessEndScene()
{
    setFixedSize(600,600);
    setWindowTitle("游戏结束");
    setWindowIcon(QIcon(":/picture_for_chess/icon.jpg"));
    restartbtn = new Mybtn(":/picture_for_chess/button1_restart.png",":/picture_for_chess/button1_restart_press.png",0.22);
    restartbtn->setParent(this);
    restartbtn->move(120,450);
    continuebtn = new Mybtn(":/picture_for_chess/button1_continue.png",":/picture_for_chess/button1_continue_press.png",0.22);
    continuebtn->setParent(this);
    continuebtn->move(350,450);
    lab = new QLabel(this);
    lab->setFont(QFont("华文新魏", 25, 75));
    lab->setStyleSheet("color:red");
    lab->move(150,200);

}

void ChessEndScene::show(bool col)
{
    if(col)
        lab->setText("游戏结束，白棋获胜");
    else
        lab->setText("游戏结束，黑棋获胜");
    QDialog::show();
}

void ChessEndScene::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/picture_for_chess/background2.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
}


