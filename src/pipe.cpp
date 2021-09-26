#include "pipe.h"
#include <QPixmap>
#include <QDebug>
Pipe::Pipe()
{
    setFixedSize(50,50);
}

void Pipe::set(int i,int s)
{
    type=i;//水管种类
    status=s;//水管状态
    switch(type)//根据种类设置图片以及根据状态设置改变后图片
    {
    //直水管0,直水管可以旋转2次
    case 0:
        upp=2;
        break;
        //十字水管1,十字水管不可旋转
    case 1:
        upp=2;
        break;
        //单个弯水管2,单个弯水管4次
    case 2:
        upp=4;
        break;
        //两个弯水管3,两个弯水管2次
    case 3:
        upp=2;
        break;
        //无水管4，可以旋转0次
    case 4:
        upp=1;
        break;
        //头水管6 可以旋转0次
    case 5:
        //尾水管5，可旋转0次
    case 6:
        upp=0;
        break;

    }
}


bool Pipe::change()
{
    //可以旋转的水管才让转
    if(type==0||type==1||type==2||type==3)
    {
        status=(status+1)%upp;
        UP();
        return true;
    }
    return false;
}


void Pipe::fill(int x,int c)//x代表某一种图片的水管形式   c代表总的水管种类
{
    setPixmap(QPixmap(QString(":/xiushuiguan/%1_%2.png").arg(type).arg(upp+(c+1)*status+x)));
}

void Pipe::UP()
{
    setPixmap(QPixmap(QString(":/xiushuiguan/%1_%2.png").arg(type).arg(status)));
}
