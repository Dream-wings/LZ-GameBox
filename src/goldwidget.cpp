#include "goldwidget.h"
#include "dataconfig.h"
#include <QIcon>
#include <QPainter>
#include <QDebug>

Goldwidget::Goldwidget(QWidget *parent) : QWidget(parent)
{
    initial();

    //记录总时间

    load();//载入关卡

    swing(100);//钩子摇晃
    whole->start(1000);
}

void Goldwidget::initial()
{
    //初始化各种状态量
    count=time=0,dir=5;
    target=nullptr,level=-1,grades=0;
    ret=1;
    speed=4;

    //初始化背景
    setFixedSize(1080,720);
    setWindowIcon(QIcon(":/res/icon.jpg"));
    setWindowTitle("黄金矿工");

    //初始化返回按钮
    backbtn=new Mybtn(":/res/backbtnr.png",":/res/backbtnp.png");
    connect(backbtn,&QPushButton::clicked,[=](){
        QTimer::singleShot(200,[=](){
           emit back();
        });
    });
    backbtn->setParent(this);
    backbtn->move(730,25);


    //初始化计时器
    st=new QTimer(this);
    whole=new QTimer(this);
    //定时器链接
    connect(st,&QTimer::timeout,[=](){
        static int q=-1;
        target=nullptr;
        g->init(dir+=q);
        if(dir==10)//摇摆到底则反方向摇摆
            q=-1;
        else if(!dir)
            q=1;
    });
    connect(whole,&QTimer::timeout,[=](){
        --time;
        if(!shooting)
        {
            //如果超时了则失败  返回上一层界面
            if(time<=0)
            {
                shooting=1;
                whole->stop();
                lose.raise();
                lose.show();
                QTimer::singleShot(3000,[=](){
                    lose.hide();
                    emit back();
                });
                return;
            }
            else if(time<=10)//倒计时快要结束时变为红色
                TimeLabel->setStyleSheet("color:red");
            //输出时间改变量
            if(last!=-1&&last-time<2)
                TimeLabel->setText(QString("%1 -%2").arg(time+suspend+1).arg((suspend+2)));
            else
                TimeLabel->setNum(time);
        }
        else
            ++suspend;
    });

    //初始化钩爪;
    g=new Grab(this);
    g->init(dir);//初始化为90°
    g->show();

    //初始化各种小标签;
    QPalette pa;
    TimeLabel=new QLabel(this);
    GradesLabel = new QLabel(this);
    LevelLabel = new QLabel(this);
    TargetLabel = new QLabel(this);
    TimeLabel->move(960,13);
    TimeLabel->setFixedSize(100,80);
    TimeLabel->setFont(QFont("Microsoft YaHei", 20, 75));
    TimeLabel->setAlignment(Qt::AlignLeft);
    TimeLabel->show();
    LevelLabel->move(923,55);
    LevelLabel->setFixedSize(70,80);
    LevelLabel->setFont(QFont("Microsoft YaHei", 20, 75));
    LevelLabel->setAlignment(Qt::AlignLeft);
    LevelLabel->show();
    TargetLabel->move(147,65);
    TargetLabel->setFixedSize(150,80);
    TargetLabel->setFont(QFont("华文新魏", 20, 75));
    TargetLabel->setAlignment(Qt::AlignLeft);
    TargetLabel->show();
    GradesLabel->move(115,15);
    GradesLabel->setFixedSize(150,80);
    GradesLabel->setFont(QFont("华文新魏", 25, 75));
    pa.setColor(QPalette::WindowText,Qt::green);
    GradesLabel->setAlignment(Qt::AlignLeft);
    GradesLabel->setNum(grades);
    GradesLabel->show();

    //初始化胜利图片
    QPixmap pix;
    pix.load(":/res/success.png");
    success.setParent(this);
    success.setFixedSize(QSize(pix.width(),pix.height()));
    success.setPixmap(pix);
    success.move(0,0);
    success.hide();
    //初始化失败图片
    pix.load(":/res/lose.png");
    lose.setParent(this);
    lose.setFixedSize(QSize(pix.width(),pix.height()));
    lose.setPixmap(pix);
    lose.move(0,0);
    lose.hide();

    //初始化音乐按钮

}

void Goldwidget::swing(int msec)
{
    st->start(msec);
}

void Goldwidget::load()
{
    //初始化页面
    ++level;
    dataconfig config;

    for(auto i:tmp)//清除页面上的所有矿石
        delete i;
    tmp.clear();

    Obj * p;
    last=-1;

    //读入关卡配置
    time=config.time[level];
    for(int i=0;i<config.sum[level];++i)
    {
        p=new Obj(config.data[level][i].k,config.data[level][i].id,config.data[level][i].r);
        tmp.push_back(p);
        p->setParent(this);
        p->show();
        mine[config.data[level][i].k].push_back(p);
    }

    //重新设置关卡、时间、以及目标分数
    TimeLabel->setStyleSheet("color:black");
    TimeLabel->setNum(time);
    LevelLabel->setNum(level+1);
    TargetLabel->setNum(650+545*level+135*(level+1)*level);//目标分数生成代码

    shooting=false;
}

void Goldwidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        if(!shooting)
        {
            shooting=1;//标记为出勾状态，避免重复点击
            st->stop();//摇摆停止
            suspend=0;
            QTimer *tmp=new QTimer(this);//定义出勾的计时器
            //检测方向上是否有矿石
            if(mine[dir].size())
            {
                int t=2000,index=0;
                for(int i=0;i<mine[dir].size();++i)
                    if(mine[dir][i]->r<t)
                    {
                        index=i;
                        t=mine[dir][i]->r;
                    }
                target=mine[dir][index];
                for(auto &i:this->tmp)
                    if(i==target)
                        i=nullptr;
                mine[dir].erase(mine[dir].begin()+index,mine[dir].begin()+index+1);
                speed=target->speed;
            }
            //出勾开始
            tmp->start(75);
            connect(tmp,&QTimer::timeout,[=](){


                if(g->y()>690||g->x()<30||g->x()>1050)
                    ret=-1;//如果越界则返回

                if(ret==-1&&g->y()==Grab::standard[dir].pos.y())//如果回到原来的起点
                {
                    tmp->stop(),ret=1;//停止当前计时器
                    last=time;
                    if(target)
                    {
                        //将速度设置成为默认值
                        speed=4;
                        //更新分数
                        GradesLabel->setNum(grades+=target->rank);
                        target->hide();
                        //讨论通关情况
                        if(grades>=650+545*level+135*(level+1)*level)
                        {
                            whole->stop();
                            success.raise();
                            success.show();
                            QTimer::singleShot(3000,[=](){
                                success.hide();
                                load();
                                whole->start();
                            });
                        }
                        delete target;
                        target=nullptr;
                    }
                    swing(100);//令钩子重新摇摆
                    shooting=0;

                    return;
                }
                //ret代表方向  speed是出勾速度
                if(target&&ret==-1)
                    target->move(target->x()+Grab::standard[dir].dx*speed*ret,target->y()+Grab::standard[dir].dy*speed*ret);

                update();//更新绳索的动画
                g->move(g->x()+Grab::standard[dir].dx*speed*ret,g->y()+Grab::standard[dir].dy*speed*ret);//更新钩子的动画
            });
        }
    }
    return QWidget::mousePressEvent(event);
}

void Goldwidget::paintEvent(QPaintEvent*)
{
    QPainter paint(this);
    paint.setRenderHint(QPainter::Antialiasing);
    QPen pen;
    pen.setWidth(2);
    pen.setColor(QColor(41,41,41));
    paint.setPen(pen);

    //设置背景
    QPixmap pix;
    bool re =pix.load(":/res/goldgamebg.png");
    if(!re)
        qDebug()<<"黄金矿工游戏背景图加载出现异常";
    paint.drawPixmap(0,0,pix);

    //设置出勾动画
    if(shooting)
    {
        count+=ret;
        int x=543+Grab::standard[dir].dx*count*speed,y=96+Grab::standard[dir].dy*count*speed;
        //如果目标存在且已经抵达则返回
        if(target&&count*speed==target->r)
            ret=-1;
        paint.drawLine(543,96,x,y);
    }
    else
        count=0;
}
