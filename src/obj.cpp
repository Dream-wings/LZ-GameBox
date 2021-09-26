#include "obj.h"
#include "grab.h"
#include <cstdlib>
#include <ctime>
#include <QDebug>

Obj::Obj(int k,int id,int r):r(r)
{
    QPixmap pix;
    bool ret;
    switch(id)
    {
    case 0://大金块
        rank=500;
        speed=1;
        ret=pix.load(":/res/gold1.png");
        break;
    case 1://小金块
        rank=100;
        speed=3;
        ret=pix.load(":/res/gold2.png");
        break;
    case 2://迷你金块
        rank=10;
        speed=7;
        ret=pix.load(":/res/gold3.png");
        break;
    case 3://石头
        rank=-10;
        speed=3;
        ret=pix.load(":/res/stone.png");
        break;
    case 4:
        rank=-50;
        speed=2;
        ret=pix.load(":/res/stone2.png");
        break;
    case 5://小口袋
        srand(time(0));
        rank= rand()%500;
        speed=4;
        ret=pix.load(":/res/pocket.png");
        break;
    case 6:
        rank=600;
        speed=6;
        ret=pix.load(":/res/diamond.png");
        break;
    }
    setPixmap(pix);
    if(!ret)
        qDebug()<<"矿石图片载入出错"<<endl;
    pos=QPoint(543+r*Grab::standard[k].dx-pix.width()/2,96+r*Grab::standard[k].dy-pix.height()/2);
    move(pos);
}
