#include "register_success_scene.h"

Register_success_scene::Register_success_scene()
{
    setWindowTitle("注册成功窗口");
    setWindowIcon(QIcon(":/choosescene/mainicon.png"));
    setFixedSize(600,300);

    success_text = new QLabel(this);
    success_text->setFixedSize(600,200);
    success_text->setAlignment(Qt::AlignCenter);
    success_text->setFont(QFont("宋体", 25, 75));
    success_text->setText("注册账号成功，3秒后为你返回登录界面");
    success_text->move((width()-success_text->width())/2,(height()-success_text->height())/2);
    success_text->setStyleSheet("color:white");
}
void  Register_success_scene::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QPixmap pix;
    bool ret=pix.load(":/res/FailToLoginScene.jpg");
    if(!ret)
        qDebug()<<"图片加载出现错误"<<endl;
    else
        painter.drawPixmap(0,0,this->width(),this->height(),pix);
}

