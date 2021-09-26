#include "register_fail_scene.h"

Register_fail_scene::Register_fail_scene()
{
    setWindowTitle("注册失败");
            setWindowIcon(QIcon(":/choosescene/mainicon.png"));
    setFixedSize(500,300);

    success_text = new QLabel(this);
    success_text->setAlignment(Qt::AlignCenter);
    success_text->setFont(QFont("华文新魏", 25, 75));
    success_text->setText("注册失败\n请务必注意保证两次输入相同！");
    success_text->setFixedSize(500,200);
    success_text->move((width()-success_text->width())/2,(height()-success_text->height())/2);
    success_text->setStyleSheet("color:red");
}
void  Register_fail_scene::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QPixmap pix;
    bool ret=pix.load(":/res/FailToLoginScene.jpg");
    if(!ret)
        qDebug()<<"图片加载出现错误"<<endl;
    else
        painter.drawPixmap(0,0,this->width(),this->height(),pix);
}
