#include "mybtn.h"
#include <QPropertyAnimation>
#include <QDebug>
#include <QTimer>

Mybtn::Mybtn(QString a,QString b,double ratio):ratio(ratio)
{
    ImgPath[0]=a,ImgPath[1]=b;
    load(a);
    player=new QMediaPlayer(this);
    player->setMedia(QUrl("qrc:/picture_for_chess/click_m.m4a"));
}

void Mybtn::load(QString a)
{
    QPixmap pix;
    bool ret=pix.load(a);
    if(!ret)
        qDebug()<<"Mybtn图片加载出现错误"<<a<<endl;
    pix=pix.scaled(ratio*pix.width(),ratio*pix.height(),Qt::KeepAspectRatio);
    //按钮样式初始化
    setFixedSize(pix.width(),pix.height());
    setStyleSheet("QPushButton{border:0px}");
    setIcon(pix);
    setIconSize(QSize(pix.width(),pix.height()));
}

void Mybtn::zoom()
{
    QPropertyAnimation* animation =new QPropertyAnimation(this,"geometry");
    //动画参数设置
    //向下弹跳
    animation->setDuration(200);
    animation->setEasingCurve(QEasingCurve::OutBounce);
    animation->setStartValue(QRect(x(),y(),height(),width()));
    animation->setEndValue(QRect(x(),y()+10,height(),width()));
    animation->start();
    QTimer::singleShot(100,[=](){
        animation->setStartValue(QRect(x(),y(),height(),width()));
        animation->setEndValue(QRect(x(),y()-10,height(),width()));
        animation->start();
    });
    //向上弹跳
}

void Mybtn::mousePressEvent(QMouseEvent* e)
{
    player->play();
    if(ImgPath[1]!="")
    {
        load(ImgPath[1]);
        //设置中心缩放
        move(x()+(QPixmap(ImgPath[0]).height()-QPixmap(ImgPath[1]).height())/2,y()+(QPixmap(ImgPath[0]).width()-QPixmap(ImgPath[1]).width())/2);

    }

    return QPushButton::mousePressEvent(e);
}
void Mybtn::mouseReleaseEvent(QMouseEvent* e)
{
    if(ImgPath[1]!="")
    {
        load(ImgPath[0]);
        move(x()-(QPixmap(ImgPath[0]).height()-QPixmap(ImgPath[1]).height())/2,y()-(QPixmap(ImgPath[0]).width()-QPixmap(ImgPath[1]).width())/2);
    }
    return QPushButton::mouseReleaseEvent(e);
}
