#include "grab.h"
#include <QDebug>

Revolve Grab::standard[11];

Grab::Grab(QWidget *parent) : QLabel(parent)
{
//90度
    standard[5].dx=0;
    standard[5].dy=4;
    standard[5].Grabpath=":/res/grab90.png";
    standard[5].pos=QPoint(522,87);
//104度
    standard[4].dx=-1;
    standard[4].dy=4;
    standard[4].Grabpath=":/res/grab104.png";
    standard[4].pos=QPoint(518,86);
//116度
    standard[3].dx=-2;
    standard[3].dy=4;
    standard[3].Grabpath=":/res/grab116.png";
    standard[3].pos=QPoint(515,85);
//123.7度
    standard[2].dx=-2;
    standard[2].dy=3;
    standard[2].Grabpath=":/res/grab123.7.png";
    standard[2].pos=QPoint(512,89);
//135度
    standard[1].dx=-2;
    standard[1].dy=2;
    standard[1].Grabpath=":/res/grab135.png";
    standard[1].pos=QPoint(512,79);
//154度
    standard[0].dx=-4;
    standard[0].dy=2;
    standard[0].Grabpath=":/res/grab154.png";
    standard[0].pos=QPoint(511,85);
//76度
    standard[6].dx=1;
    standard[6].dy=4;
    standard[6].Grabpath=":/res/grab76.png";
    standard[6].pos=QPoint(529,86);
//64度
    standard[7].dx=2;
    standard[7].dy=4;
    standard[7].Grabpath=":/res/grab64.png";
    standard[7].pos=QPoint(531,85);
//56.3度
    standard[8].dx=2;
    standard[8].dy=3;
    standard[8].Grabpath=":/res/grab64.png";
    standard[8].pos=QPoint(531,85);
//45度
    standard[9].dx=2;
    standard[9].dy=2;
    standard[9].Grabpath=":/res/grab45.png";
    standard[9].pos=QPoint(534,86);
//26度
    standard[10].dx=4;
    standard[10].dy=2;
    standard[10].Grabpath=":/res/grab26.png";
    standard[10].pos=QPoint(536,84);

}

void Grab::init(int i)
{
    QPixmap pix;
    bool ret=pix.load(standard[i].Grabpath);
    if(!ret)
        qDebug()<<"钩子图片加载异常";
    setFixedSize(QSize(pix.width(),pix.height()));
    setPixmap(pix);
    move(standard[i].pos);
}
