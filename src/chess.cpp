#include "chess.h"
#include "wuziqi.h"
#include "QDebug"

QString const Chess::cpath[3]={":/picture_for_chess/black_item.png",":/picture_for_chess/white_item.png",""};

Chess::Chess(int x,int y,bool col,QWidget* parent):QLabel(parent)
{
    pos.setX(x),pos.setY(y);
    bool ret;
    QPixmap pix;
    ret=pix.load(cpath[col]);
    pix=pix.scaled(QSize(25,25),Qt::IgnoreAspectRatio);
    if(!ret)
        qDebug()<<"棋子图片加载失败";
    setFixedSize(pix.width(),pix.height());
    setPixmap(pix);
    QPoint t=Wuziqi::transform(x,y);
    move(t.x()-pix.width()/2,t.y()-pix.height()/2);
    show();
}
