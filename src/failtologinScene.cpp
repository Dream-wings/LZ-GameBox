#include "failtologinScene.h"
#include <QLabel>
#include <QPainter>
#include <QIcon>
#include <QDebug>
FailToLogin::FailToLogin()
{
    setFixedSize(600,300);
    setWindowTitle("登录错误提醒");
        setWindowIcon(QIcon(":/choosescene/mainicon.png"));
    QLabel *failtologin=new QLabel(this);
    failtologin->setFont(QFont("华文新魏", 25, 75));
    failtologin->setAlignment(Qt::AlignCenter);
    failtologin->setText("你输入的账号或密码有问题\n请重新输入！");
    failtologin->setFixedSize(500,200);
    failtologin->move((width()-failtologin->width())/2,(height()-failtologin->height())/2);
    failtologin->setStyleSheet("color:red;");
}
void  FailToLogin::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QPixmap pix;
    bool ret=pix.load(":/res/FailToLoginScene.jpg");
    if(!ret)
        qDebug()<<"图片加载出现错误"<<endl;
    else
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
}
