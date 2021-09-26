#include "wuziqi.h"
#include "mybtn.h"
#include <QIcon>
#include <QPainter>
#include <QPixmap>
#include <QPaintEvent>
#include <QRect>
#include <QPen>
#include <QLabel>
#include <QColor>
#include <QFont>
#include <QBrush>
#include <QDebug>
#include <QtCore/qmath.h>
#include <QMessageBox>
#include <cstring>
#include <QTime>
#include <QtNetwork>
#include <algorithm>
using std::sort;

template<typename T>
T Min(const T &a,const T &b)
{
    return a<b?a:b;
}
template<typename T>
T Max(const T &a,const T &b)
{
    return a>b?a:b;
}

//AI
int repeat[361],priority[361];
//优先级转化函数
//优先级
//己方4子>对方4子>对方3子中间的空位>我方3子中间的空位>对方3子>己方3子>对方2子>己方2子>己方1子>对方1子
//   10     9         8                7         6      5      4      3      2      1
//取出优先级最高的点位
int trans_prio(bool enemy,int len)
{
    if(enemy)
    {
        switch(len)
        {
        case 1:return 1;
        case 2:return 4;
        case 3:return 6;
        default:return 9;
        }
    }
    else
    {
        switch(len)
        {
        case 1:return 2;
        case 2:return 3;
        case 3:return 5;
        default:return 10;
        }
    }
    return -1;
}
int Hash(int x,int y)
{
    return x*19+y;
}
bool Wuziqi::Erase(int x)
{
    for(auto i=steps.begin();i!=steps.end();++i)
        if(x==i->pos)
        {
            steps.erase(i);
            return true;
        }
    return false;
}

AIsteps::AIsteps(int xx,int yy)
{
    x=xx,y=yy,pos=Hash(xx,yy);
}
AIsteps::AIsteps(int xx)
{
    x=xx/19,y=xx%19,pos=xx;
}

bool AIsteps::operator<(const AIsteps& a)const
{
    if(priority[pos]!=priority[a.pos])
        return priority[pos]>priority[a.pos];
    if(repeat[pos]!=repeat[a.pos])
        return repeat[pos]>repeat[a.pos];
    return pos<a.pos;
}



//  udp传输
//type 0代表准备信号 type 1代表悔棋信号  type 2代表下棋信号  type 3代表重玩信号  type 4代表继续信号  type 5 代表投降信号  type 6 代表绑定信号
//悔棋type2 -1代表询问信号  0 代表拒绝信号  1代表同意信号
//type 3 会夹带是否胜利 下棋位置
enum Type{ready,regret,play,restart,conti,giveup,bind};//三种数据信号
//谁最先点准备 谁是白棋（先走）

typedef QByteArray B;

Wuziqi::Wuziqi(QWidget *parent)
    : QWidget(parent)
{
    init();

    //设置窗口界面
    setFixedSize(1080,720);
    setWindowIcon(QIcon(":/picture_for_chess/icon.jpg"));
    setWindowTitle("五子棋");

    //初始化计时、分数等标签
    time=new QTimer(this);
    connect(time,&QTimer::timeout,[=](){
        --tim[doing];
        TimeRecorder->setText(QString("计时：%1:%2        ").arg(tim[1]/60,2,10,QLatin1Char('0')).arg(tim[1]%60,2,10,QLatin1Char('0'))+
                QString("%1:%2").arg(tim[0]/60,2,10,QLatin1Char('0')).arg(tim[0]%60,2,10,QLatin1Char('0')));
        if(!tim[doing])
        {
            doing=!doing;
            success();
        }
    });
    QFont font;
    font.setBold(true);
    font.setPointSize(20);
    font.setFamily("华文新魏");
    TimeRecorder->move(670,420);
    TimeRecorder->setStyleSheet("color:cyan;");
    TimeRecorder->setText("计时：10:00        10:00");
    TimeRecorder->setFixedSize(300,60);
    TimeRecorder->setFont(font);

    ScoreRecorder->move(670,500);
    ScoreRecorder->setStyleSheet("color:cyan;");
    ScoreRecorder->setText("积分：   0               0");
    ScoreRecorder->setFixedSize(300,60);
    ScoreRecorder->setFont(font);


    //头像
    people1->move(700,180);
    people2->move(900,180);

    //设置按钮
    returnbtn->setParent(this);
    regretbtn->setParent(this);
    giveupbtn->setParent(this);
    readybtn->setParent(this);
    readybtn->move(750,330);
    regretbtn->move(750,560);
    giveupbtn->move(750,640);
    returnbtn->move(1020,650);

    //遮罩以及模式选择按钮
    zhaozi->move(4,10);
    zhaozi->setFixedSize(652,682);
    zhaozi->setStyleSheet("QPushButton{border:0px}");
    zhaozi->setPixmap(QPixmap(":/choosescene/top3.png"));

    AIbtn->setParent(zhaozi);
    AIbtn->move(150,500);

    Onlinebtn->setParent(zhaozi);
    Onlinebtn->move(350,500);
    select_computer->move(22,30);
    select_person->move(22,30);

    connect(AIbtn,&Mybtn::clicked,[=](){
        zhaozi->close();
        //选择先手后手
        AImode=true;
        select_computer->show();
        //进入AI对战
        connect(select_computer->firstbtn,&QPushButton::clicked,[=](){
            playing=1;
            wait=0;
            select_computer->hide();
            readybtn->setEnabled(true);
        });
        connect(select_computer->secondbtn,&QPushButton::clicked,[=](){
            wait=1;
            playing=0;
            select_computer->hide();
            readybtn->setEnabled(true);
        });

    });
    connect(Onlinebtn,&Mybtn::clicked,[=](){
        zhaozi->close();
        //进入联机对战
        select_person->show();
        //联机模式按钮设置
        connect(select_person->bindbtn,&QPushButton::clicked,[=](){
            //书写方便
            if(select_person->bindbtn->change)
            {
                ConnectInput* p=select_person;
                QString a,b;
                a=p->friend_IP_w->text();
                b=p->friend_port_w->text();
                if(a==""||b=="")
                {
                    QMessageBox::warning(this,"警告","请不要出现空白输入");
                    select_person->bindbtn->change=0;
                    p->bindbtn->load(p->bindbtn->ImgPath[0]);
                    return;
                }
                //检查格式
                for(auto i:a)
                    if(i!='.'&&!i.isDigit())//如果不是.或者数字警告
                    {
                        QMessageBox::warning(this,"警告","请输入正确格式的IP");
                        select_person->bindbtn->change=0;
                        p->bindbtn->load(p->bindbtn->ImgPath[0]);
                        return;
                    }
                for(auto i:b)
                    if(!i.isDigit())
                    {
                        QMessageBox::warning(this,"警告","请输入正确格式的端口");
                        select_person->bindbtn->change=0;
                        p->bindbtn->load(p->bindbtn->ImgPath[0]);
                        return;
                    }
                //读入信号和端口
                IP[1]=a;
                PORT[1]=b.toInt();
                bind[0]=1;
                //发送信号
                send(Type::bind);
                qDebug()<<bind[1];
                if(bind[1])
                {
                    select_person->hide();
                    select_person->bindbtn->init();
                    QMessageBox::information(this,"提示","已连接");
                    readybtn->setEnabled(true);
                }
            }

        });
    });

    //准备信号的发送
    connect(readybtn,&Mybtn::clicked,[=](){
        if(AImode)
        {
            if(!first)
            {
                set_head();
                people1->show();
                people2->show();
            }
            wait=!playing;
            startgame();
            if(!playing)
                AI();
        }
        else
        {
            playing=!ready[1];
            ready[0]=1;
            B t;
            QDataStream out(&t,QIODevice::WriteOnly);
            //如果对方已经点击准备，则开始游戏
            if(ready[1])
            {
                out<<Type::ready;
                if(!first)
                {
                    out<<set_head();
                    people1->show();
                    people2->show();
                }
                else
                    out<<-1;

                startgame();
            }
            else
                out<<Type::ready<<-1;
            send(t);

        }
    });
    //悔棋信号的发送
    connect(regretbtn,&Mybtn::clicked,[=](){
        if(AImode)
        {
            //防止手速过快 AI还没下
            if(!wait)
            {
                ste-=wait?1:2;
                regret(!wait);
                if((playing==1&&!ste)||(playing==0&&ste==1))
                    regretbtn->setEnabled(false);
                doing=playing;
            }
        }
        else
        {
            B t;
            QDataStream out(&t,QIODevice::WriteOnly);
            out<<Type::regret<<-1;
            send(t);
        }
    });

    //返回游戏界面信号发送
    connect(returnbtn,&Mybtn::clicked,[=](){
        hide();
        restart(-1);
        time->stop();
        emit back();
        //返回游戏选择界面
        qDebug()<<"返回游戏选择界面";

    });
    //投降信号发送
    connect(giveupbtn,&Mybtn::clicked,[=](){
        if(AImode)
        {
            //投降直接结束比赛
            doing=!playing;
            success();
        }
        else
        {
            doing=!playing;
            send(Type::giveup);
            success();
        }
    });

}

void Wuziqi::init()
{
    //玩家默认设置为白棋
    playing=1;
    ces=nullptr;
    //初始化棋盘
    memset(vis,-1,sizeof(vis));
    //创捷标签
    TimeRecorder = new QLabel(this);
    ScoreRecorder = new QLabel(this);
    //创建按钮
    returnbtn = new Mybtn(":/picture_for_chess/button4_exit.png",":/picture_for_chess/button4_exit_press.png",0.14);
    regretbtn = new Mybtn(":/picture_for_chess/button2_regret.png",":/picture_for_chess/button2_regret_press.png",0.35);
    giveupbtn = new Mybtn(":/picture_for_chess/button2_giveup.png",":/picture_for_chess/button2_giveup_press.png",0.35);
    readybtn = new ConnectBtn(":/picture_for_chess/button2_ready.png",":/picture_for_chess/button2_ready_press.png",":/picture_for_chess/button2_readying.png",0.35);
    //创建头像
    people1 = new  QLabel(this);
    people2 = new  QLabel(this);
    head[0].load(":/picture_for_chess/People1.jpg");
    head[1].load(":/picture_for_chess/People2.jpg");
    head[2].load(":/picture_for_chess/People3.jpg");
    head[3].load(":/picture_for_chess/People4.jpg");
    head[4].load(":/picture_for_chess/computer.jpg");
    for(int i=0;i<5;i++)
        head[i]=head[i].scaled(120,120,Qt::KeepAspectRatio);
    //创建遮罩以及开始选择的按钮
    zhaozi = new QLabel(this);//罩子
    AIbtn = new Mybtn(":/picture_for_chess/button1_AI.png",":/picture_for_chess/button1_AI_press.png",0.22);
    Onlinebtn = new Mybtn(":/picture_for_chess/button1_online.png",":/picture_for_chess/button1_online_press.png",0.22);
    //创建先手后手场景界面
    select_computer = new FirstOrScendScene(this);
    select_computer->hide();
    select_person = new ConnectInput(this);
    select_person->hide();
    //种种子
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    //创建落子声音播放器
    player = new QMediaPlayer(this);
    player->setMedia(QUrl("qrc:/picture_for_chess/chess_m.m4a"));
}

void Wuziqi::paintEvent(QPaintEvent* )
{
    QPainter painter(this);
    QPen pen(Qt::black);
    pen.setWidth(2);
    QBrush brush(QPixmap(":/picture_for_chess/chessboard.jpg"));
    QBrush brush2(Qt::black);
    //画五子棋标语
    painter.drawPixmap(0,0,this->width(),this->height(),QPixmap(":/picture_for_chess/welcomescene.png"));
    painter.drawPixmap(660,40,400,100,QPixmap(":/picture_for_chess/logo3.png"));

    painter.setBrush(brush);
    painter.drawRect(10,20,632,662);
    painter.setPen(pen);
    //画棋盘
    for(int i=0;i<=18;i++)
    {
        painter.drawLine(transform(i,0),transform(i,18));//画列
        painter.drawLine(transform(0,i),transform(18,i));//画行
    }
    //画圆点
    painter.setBrush(brush2);
    for(int i=3;i<=15;i+=6)
        for(int j=3;j<=15;j+=6)
            painter.drawEllipse(transform(i,j),3,3);

}

void Wuziqi::solve()
{
    QByteArray str;
    QDataStream in(&str,QIODevice::ReadOnly);
    QByteArray t;
    QDataStream out(&t,QIODevice::WriteOnly);
    while(udp->hasPendingDatagrams())
    {
        str.resize(udp->pendingDatagramSize());
        udp->readDatagram(str.data(),str.size());
        in>>type;
        qDebug()<<type;
        switch(type)
        {
        case Type::ready:
            //如果发来准备信号时，自己没有准备，自己则是黑棋
            ready[1]=1;
            playing=ready[0];
            wait=!ready[0];
            //自己先手的话  开始游戏的信号是要接受对面的
            if(ready[0])
            {
                int tmp;
                in>>tmp;
                if(~tmp)
                {
                    people1->setPixmap(head[tmp/5]);
                    people2->setPixmap(head[tmp%5]);
                    people1->show();
                    people2->show();
                }
                startgame();
            }
            break;
        case Type::regret:
            int signal;
            in>>signal;
            //询问 同意  不同意三种情况进行处理
            switch(signal)
            {
            case -1:
                out<<Type::regret;
                if(QMessageBox::Ok==QMessageBox::question(this,"悔棋","对方想要悔棋，是否同意",QMessageBox::Ok|QMessageBox::No,QMessageBox::Ok))
                {
                    ste-=wait?2:1;
                    regret(wait);
                    wait=1;
                    doing=!playing;
                    out<<1;
                }
                else
                    out<<0;
                send(t);
                break;
            case 0:
                QMessageBox::warning(this,"提示","对方不同意悔棋");
                break;
            case 1:
                ste-=wait?1:2;
                //自己先手情况下没有子剩余  自己后手情况下剩余对方一个子
                if((playing==1&&!ste)||(playing==0&&ste==1))
                    regretbtn->setEnabled(false);
                qDebug()<<"悔棋";
                regret(!wait);
                doing=playing;
                wait=0;
                break;
            }
            break;
        case Type::play:
            ++ste;
            bool ok;
            int x,y;
            in>>ok>>x>>y;
            qDebug()<<ok;
            vis[x][y]=doing;
            vec.push_back(new Chess(x,y,doing,this));
            if(ok)
            {
                success();
                return;
            }
            wait=0;
            doing=!doing;
            break;
        case Type::restart:
            qDebug()<<"重玩";
            restart(-1);
            break;
        case Type::conti:
            conti[1]=1;
            if(conti[0])
                restart(doing);
            break;
        case Type::giveup:
            doing=playing;
            qDebug()<<"giveup";
            success();
            break;
        case Type::bind:
            //处于多余消息
            if(bind[1])
                return;
            bind[1]=1;
            if(bind[0])
            {
                select_person->hide();
                select_person->bindbtn->init();
                QMessageBox::information(this,"提示","已连接");
                readybtn->setEnabled(true);
            }
            break;

        }
    }
}

void Wuziqi::mousePressEvent(QMouseEvent* e)
{
    //寻找到最近的格点
    int x=e->x(),y=e->y(),tx=(x-20)%34,ty=(y-40)%34;
    if(first&&!wait&&e->x()>3&&e->x()<=649&&e->y()>20&&e->y()<=669)
    {
        x-=20,y-=40;
        x=tx<17?x/34:x/34+1;
        y=ty<17?y/34:y/34+1;
        if(vis[x][y]==-1)
        {
            ++ste;
            if(ste==1||ste==2)
                regretbtn->setEnabled(true);
            if(repeat[AIsteps(x,y).pos])
            {
                int t=AIsteps(x,y).pos;
                priority[t]=repeat[t]=0;
                Erase(t);
            }
            vis[x][y]=doing;
            vec.push_back(new Chess(x,y,doing,this));
            //判断是否胜利
            bool ok=check(x,y);
            //界面添加显示棋子
            if(!AImode)
            {
                //将信息发送给端口1
                QByteArray str;
                QDataStream out(&str,QIODevice::WriteOnly);
                out<<Type::play<<ok<<x<<y;
                send(str);
                if(ok)
                {
                    success();
                    return;
                }
                wait=1;
                doing=!doing;
            }
            else
            {
                if(ok)
                    success();
                else
                {
                    wait=1;
                    doing=!doing;
                    QTimer::singleShot(800,[=](){
                        AI();
                    });
                }
            }

        }
    }

    return QWidget::mousePressEvent(e);
}

bool Wuziqi::check(int x, int y)
{
    player->play();
    //i^1代表方向
    //下  上  右  左  右下  左上  左下  左上
    int startx,starty,endx,endy,tx,ty;
    int dir[8][2]={0,1,0,-1,1,0,-1,0,1,1,-1,-1,-1,1,1,-1};
    //AI操作时的相关设置
    bool ok[2];//忽略一次非本色棋子的机会是否用掉  0代表反方向搜索时   1代表正方向搜索时
    int aisx[2],aisy[2],aiex[2],aiey[2];//反方向走为终点  正方向走为起点  0代表没有使用机会的点  1代表使用机会的点

    for(int i=0;i<8;i+=2)
    {
        startx=endx=x;
        starty=endy=y;
        ok[0]=ok[1]=0;
        aisx[1]=aiex[1]=-1;
        aisx[0]=aiex[0]=x;
        aisy[0]=aiey[0]=y;
        for(int j=1;j<=4;++j)
        {
            tx=x+j*dir[i^1][0],ty=y+j*dir[i^1][1];
            if(!legal(tx,ty)||vis[tx][ty]==!doing)//如果越界直接退出  或者碰到对手的棋子
                break;
            else if(vis[tx][ty]!=doing)//不越界的话看是否机会已使用
            {
                if(!ok[0])
                    ok[0]=1;
                else
                    break;
            }
            else if(vis[tx][ty]==doing)
            {
                if(!ok[0])
                    startx=tx,starty=ty;
                //每次根据是否使用机会将端点放进去
                aisx[ok[0]]=tx+dir[i^1][0],aisy[ok[0]]=ty+dir[i^1][1];
            }
        }
        for(int j=1;j<=4;++j)
        {
            tx=x+j*dir[i][0],ty=y+j*dir[i][1];
            if(!legal(tx,ty)||vis[tx][ty]==!doing)
                break;
            else if(vis[tx][ty]!=doing)
            {
                if(!ok[1])
                    ok[1]=1;
                else
                    break;
            }
            if(vis[tx][ty]==doing)
            {
                if(!ok[1])
                    endx=tx,endy=ty;
                //每次根据是否使用机会将端点放进去
                aiex[ok[1]]=tx+dir[i][0],aiey[ok[1]]=ty+dir[i][1];
            }
        }
        //反方向如果没同色棋子的话
        if(aisx[1]==-1&&ok[0])
            ok[0]=0;
        if(aisx[0]==x&&aisy[0]==y)
            aisx[0]=x+dir[i^1][0],aisy[0]=y+dir[i^1][1];
        //正方向如果没同色棋子的话
        if(aiex[1]==-1&&ok[1])
            ok[1]=0;
        if(aiex[0]==x&&aiey[0]==y)
            aiex[0]=x+dir[i][0],aiey[0]=y+dir[i][1];
        int len;
        bool sure;
        AIsteps t;
        //反方向上的情况
        if(ok[0])
        {
            len=Max(aiey[0]-aisy[1]-2,aiex[0]-aisx[1]-2);
            len=trans_prio(!wait,len);
            sure=~vis[aisx[1]][aisy[1]]||~vis[aiex[0]][aiey[0]];
            if(sure)
                len-=2;
            //起始点
            if(legal(aisx[1],aisy[1])&&vis[aisx[1]][aisy[1]]==-1)
            {
                t=AIsteps(Hash(aisx[1],aisy[1]));
                if(!repeat[t.pos])
                    steps.push_back(t);
                ++repeat[t.pos];
                priority[t.pos]=Max(len,priority[t.pos]);
            }
            if(legal(aiex[0],aiey[0])&&vis[aiex[0]][aiey[0]]==-1)
            {
                t=AIsteps(aiex[0],aiey[0]);
                if(!repeat[t.pos])
                    steps.push_back(t);
                ++repeat[t.pos];
                priority[t.pos]=Max(len,priority[t.pos]);
            }
            if(legal(aisx[0],aisy[0])&&vis[aisx[0]][aisy[0]]==-1)
            {
                t=AIsteps(aisx[0],aisy[0]);
                if(!repeat[t.pos])
                    steps.push_back(t);
                ++repeat[t.pos];
                priority[t.pos]=Max((wait?(len==5?7:len):(len==6?8:len))-sure*2,priority[t.pos]);
            }
        }
        //正方向上的情况
        if(ok[1])
        {
            len=Max(aiey[1]-aisy[0]-2,aiex[1]-aisx[0]-2);
            len=trans_prio(!wait,len);
            sure=~vis[aisx[0]][aisy[0]]||~vis[aiex[1]][aiey[1]];
            if(sure)
                len-=2;
            //起始点
            if(legal(aisx[0],aisy[0])&&vis[aisx[0]][aisy[0]]==-1)
            {
                t=AIsteps(Hash(aisx[0],aisy[0]));
                if(!repeat[t.pos])
                    steps.push_back(t);
                ++repeat[t.pos];
                priority[t.pos]=Max(len,priority[t.pos]);
            }
            if(legal(aiex[1],aiey[1])&&vis[aiex[1]][aiey[1]]==-1)
            {
                t=AIsteps(aiex[1],aiey[1]);
                if(!repeat[t.pos])
                    steps.push_back(t);
                ++repeat[t.pos];
                priority[t.pos]=Max(len,priority[t.pos]);
            }
            if(legal(aiex[0],aiey[0])&&vis[aiex[0]][aiey[0]]==-1)
            {
                t=AIsteps(aiex[0],aiey[0]);
                if(!repeat[t.pos])
                    steps.push_back(t);
                ++repeat[t.pos];
                priority[t.pos]=Max((wait?(len==5?7:len):(len==6?8:len))-sure*2,priority[t.pos]);
            }
        }
        if(!ok[1]&&!ok[0])
        {
            len=Max(aiey[0]-aisy[0]-1,aiex[0]-aisx[0]-1);
            len=trans_prio(!wait,len);
            sure=~vis[aisx[0]][aisy[0]]||~vis[aisx[0]][aisy[0]];
            //如果棋子有一端已经被堵住了优先级-2
            if(sure)
                len-=2;
            //起始点
            if(legal(aisx[0],aisy[0])&&vis[aisx[0]][aisy[0]]==-1)
            {
                t=AIsteps(aisx[0],aisy[0]);
                if(!repeat[t.pos])
                    steps.push_back(t);
                ++repeat[t.pos];
                priority[t.pos]=Max(len,priority[t.pos]);
            }
            if(legal(aiex[0],aiey[0])&&vis[aiex[0]][aiey[0]]==-1)
            {
                t=AIsteps(aiex[0],aiey[0]);
                if(!repeat[t.pos])
                    steps.push_back(t);
                ++repeat[t.pos];
                priority[t.pos]=Max(len,priority[t.pos]);
            }
        }
        if(Max(endx-startx,endy-starty)>=4)
            return true;
    }
    return false;
}

//每次doing是胜利者
void Wuziqi::success()
{
    time->stop();
    //弹出游戏结束消息
    ces=new ChessEndScene;
    ces->show(doing);
    //如果在AI模式下  则把AI相关数组初始化
    if(AImode)
    {
        steps.clear();
        memset(priority,0,sizeof(priority));
        memset(repeat,0,sizeof(repeat));
    }

    //重玩  任何一人点了重玩游戏都重新开始
    connect(ces->restartbtn,&Mybtn::clicked,[=](){
        readybtn->show();
        if(!AImode)
            send(Type::restart);
        restart(-1);
    });
    //继续
    connect(ces->continuebtn,&Mybtn::clicked,[=](){
        if(AImode)
            restart(doing);
        else
        {
            conti[0]=1;
            send(Type::conti);
            if(conti[1])
                restart(doing);
        }
    });
}

void Wuziqi::AI()
{
    for(auto i:steps)
        qDebug()<<i.x<<i.y<<priority[i.pos]<<repeat[i.pos];
    ++ste;
    if(steps.empty())
        steps.push_back(Hash(9,9));
    sort(steps.begin(),steps.end());
    auto t=steps.begin();
    vis[t->x][t->y]=!playing;
    repeat[t->pos]=0;
    priority[t->pos]=0;
    vec.push_back(new Chess(t->x,t->y,!playing,this));
    if(check(t->x,t->y))
        success();
    else
    {
        steps.pop_front();
        doing=!doing;
        wait=0;
    }
}

void Wuziqi::restart(int x)
{
    //使界面不可点   时间  各种信号情况初始化
    ste=0;
    wait=1;
    bind[0]=bind[1]=conti[0]=conti[1]=ready[0]=ready[1]=0;
    tim[0]=tim[1]=600;
    doing=1;
    //按钮全部禁用
    regretbtn->setEnabled(false);
    giveupbtn->setEnabled(false);

    //选择界面隐藏
    if(ces)
    {
        ces->hide();
        delete ces;
        ces=nullptr;
    }
    TimeRecorder->setText("计时：10:00        10:00");



    //棋子清除
    for(auto& it:vec)
    {
        vis[it->pos.x()][it->pos.y()]=-1;
        it->hide();
        delete it;
    }
    vec.clear();
    if(~x)
    {
        wait=0;
        ++rank[x];
        //增加获胜方的点数
        readybtn->show();
    }
    else
    {
        readybtn->setEnabled(false);
        AImode=0;
        first=0;
        people1->hide();
        people2->hide();
        rank[0]=rank[1]=0;
        bind[0]=bind[1]=0;
        zhaozi->show();
        //重新输入链接信息
    }

    ScoreRecorder->setText(QString("积分：白 %1           黑 %2").arg(rank[1],2).arg(rank[0],2));
}

void Wuziqi::send(QByteArray t)
{
    udp->writeDatagram(t,t.size(),QHostAddress(IP[1]),PORT[1]);
}

void Wuziqi::send(int x)
{
    QByteArray t;
    QDataStream out(&t,QIODevice::WriteOnly);
    t.clear();
    out<<x;
    udp->writeDatagram(t,t.size(),QHostAddress(IP[1]),PORT[1]);
}

void Wuziqi::startgame()
{
    readybtn->init();
    readybtn->hide();
    giveupbtn->setEnabled(true);
    time->start(1000);
    if(!first)
        first=1;
}

int Wuziqi::set_head()
{
    //随机生成两个头像
    QPixmap random1,random2;
    int a,b;
    a=qrand()%4;
    b=a;
    random1=head[a];
    if(AImode)
        random2=head[4];
    else
    {
        while((b=qrand()%4)==a);
        random2=head[b];
    }
    people1->setFixedSize(random1.width(),random1.height());
    people1->setStyleSheet("QPushButton{border:0px}");
    people1->setPixmap(random1);
    people2->setFixedSize(random2.width(),random2.height());
    people2->setStyleSheet("QPushButton{border:0px}");
    people2->setPixmap(random2);
    return a*5+b;
}

void Wuziqi::regret(bool x)
{
    if(x)// 悔两步
    {
        vis[(vec.back()->pos).x()][(vec.back()->pos).y()]=-1;
        vec.back()->hide();
        delete vec.back();
        vec.pop_back();
        vis[(vec.back()->pos).x()][(vec.back()->pos).y()]=-1;
        vec.back()->hide();
        delete vec.back();
        vec.pop_back();
    }
    else
    {
        vis[(vec.back()->pos).x()][(vec.back()->pos).y()]=-1;
        vec.back()->hide();
        delete vec.back();
        vec.pop_back();
    }
    //还原AI策略步数
    if(AImode)
    {

        steps.clear();
        memset(priority,0,sizeof(priority));
        memset(repeat,0,sizeof(repeat));
        bool t=doing;
        doing=1;
        for(auto i:vec)
        {
            check(i->pos.x(),i->pos.y());
            doing=!doing;
        }
        doing=t;
    }
}

bool Wuziqi::legal(int x, int y)
{
    return x>=0&&y>=0&&x<19&&y<19;
}


QPoint Wuziqi::transform(int x,int y)
{
    return QPoint(20+x*34,40+y*34);
}
