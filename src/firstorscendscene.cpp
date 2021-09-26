#include "firstorscendscene.h"
#include <QLabel>
#include <QPixmap>
#include <QIcon>
FirstOrScendScene::FirstOrScendScene(QWidget *parent):QWidget(parent)
{

    setWindowTitle("人机");
    setFixedSize(600,600);
    setWindowIcon(QIcon(":/picture_for_chess/icon.jpg"));
    firstbtn = new Mybtn(":/picture_for_chess/button3_first.png",":/picture_for_chess/button3_first_press.png",1);
    secondbtn = new Mybtn(":/picture_for_chess/button3_second.png",":/picture_for_chess/button3_second_press.png",1);
    firstbtn->setParent(this);
    secondbtn->setParent(this);
    firstbtn->move(85,320);
    secondbtn->move(345,320);


}
void  FirstOrScendScene::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,this->width(),this->height(),QPixmap(":/picture_for_chess/background2_.png"));
}
