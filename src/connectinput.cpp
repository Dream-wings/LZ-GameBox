#include "connectinput.h"
#include <QFont>
#include <QIcon>
#include <QDebug>
#include <QPainter>

//绑定按键
ConnectBtn::ConnectBtn(QString a,QString b,QString c,double ratio):Mybtn(a,b,ratio),conpath(c)
{
    init();
}

void ConnectBtn::init()
{
    change=0;
    load(ImgPath[0]);
}

void ConnectBtn::mousePressEvent(QMouseEvent *e)
{
    if(!change)
    {
        load(ImgPath[1]);
        change=1;

    }
    else
        change=0;
    return QPushButton::mousePressEvent(e);
}

void ConnectBtn::mouseReleaseEvent(QMouseEvent *e)
{
    if(change)
        load(conpath);
    else
        load(ImgPath[0]);

    return QPushButton::mouseReleaseEvent(e);
}

//连接画面
ConnectInput::ConnectInput(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("联机");
    setFixedSize(600,600);
    setWindowIcon(QIcon(":/picture_for_chess/icon.jpg"));

    //申请空间
    friend_IP=new QLabel(this);
    friend_port=new QLabel(this);
    my_IP=new QLabel(this);
    my_port=new QLabel(this);
    friend_IP_w=new QLineEdit(this);
    friend_port_w=new QLineEdit(this);
    bindbtn=new ConnectBtn(":/picture_for_chess/button3_bind.png",":/picture_for_chess/button3_bind_press.png",":/picture_for_chess/button3_bing.png");


    QFont font("宋体",15,55,true);
    font.setBold(true);
    my_IP->setFont(font);
    my_port->setFont(font);
    my_IP->move(30,this->height()-65);
    my_port->move(30,this->height()-40);
    friend_IP->setText("好友IP：");
    friend_port->setText("好友端口：");
    //设置IP最大长度
    friend_IP_w->setMaxLength(15);
    friend_IP_w->setPlaceholderText("好友界面左下角的IP地址");
    //设置端口最大长度
    friend_port_w->setMaxLength(5);
    friend_port_w->setPlaceholderText("好友界面左下角的端口地址");
    friend_IP->move(70,100);
    friend_IP_w->move(150,100);
    friend_port->move(70,150);
    friend_port_w->move(150,150);
    //按钮设置
    bindbtn->setParent(this);
    bindbtn->move(300,120);
}

void ConnectInput::myupdate()
{
    my_IP->setText(QString("IP:")+my_IP_s);
    my_port->setText(QString("端口:")+my_port_s);
}

void ConnectInput::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,this->width(),this->height(),QPixmap(":/picture_for_chess/background2.png"));
}
