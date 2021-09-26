#include "piperepairer.h"
#include "ui_piperepairer.h"
#include <QIcon>
#include <QPixmap>
#include <QDebug>
#include <QMovie>
#include <QUrl>
#include <QTime>
#include <cmath>
#include <QDialog>

//方向数组
int dir[4][2]={0,-1,1,0,0,1,-1,0};

PipeRepairer::PipeRepairer( QMainWindow *parent) :QMainWindow(parent),
    ui(new Ui::PipeRepairer)
{
    ui->setupUi(this);
    setFixedSize(1080,720);
    setWindowIcon(QIcon(":/xiushuiguan/icon.jpg"));
    setWindowTitle("修水管");

    //设置按钮
    stopbtn = new Mybtn(":/xiushuiguan/stopbtn.png",":/xiushuiguan/stopbtn.png",0.1);
    continuebtn = new Mybtn(":/xiushuiguan/continuebtn.png",":/xiushuiguan/continuebtn.png",0.1);
    mutebtn = new Mybtn(":/xiushuiguan/mutebtn.png",":/xiushuiguan/mutebtn.png",0.1);
    stopbtn->setParent(this);
    continuebtn->setParent(this);
    mutebtn->setParent(this);
    stopbtn->move(120,680);
    continuebtn->move(230,680);
    mutebtn->move(340,680);
    //设置播放视频
    player = new QMediaPlayer(this);
    videoWidget = new QVideoWidget(this);
    medialist=new QMediaPlaylist(this);
    ui->vlayout->addWidget(videoWidget);
    medialist->addMedia(QUrl("qrc:/xiushuiguan/video.avi"));
    player->setVideoOutput(videoWidget);
    medialist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    connect(stopbtn,SIGNAL(clicked()),this,SLOT(stopVideo()));
    connect(continuebtn,SIGNAL(clicked()),this,SLOT(continueVideo()));
    connect(mutebtn,SIGNAL(clicked()),this,SLOT(muteVideo()));
    //失败界面配置
    endscene = new PipeRepairerEndScene;
    connect(endscene->exitbtn,&QPushButton::clicked,[=](){
        endscene->hide();
        hide();
        stopVideo();
        emit back();
    });
    connect(endscene->restartbtn,&QPushButton::clicked,[=](){
        endscene->hide();
        level=0;
        random();
    });
    //选择界面
    choscene=new ChoiceOfDifficultyScene(this);
    choscene->hide();
    //种种子
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    for(int i=0;i<11;++i)
        for(int j=0;j<13;++j)
        {
            pipes[i][j].setParent(this);
            pipes[i][j].move(transform(i,j));
            pipes[i][j].hide();
        }
    //点击开始按钮
    wait=0;

    //初始化各种标签
    level=0;
    TimeLabel=new QLabel(this);
    TimeLabel->move(300,505);
    TimeLabel->setFont(QFont("华文新魏", 25, 75));
    TimeLabel->setStyleSheet("color:brown");
    StepLabel=new QLabel(this);
    StepLabel->move(300,560);
    StepLabel->setFont(QFont("华文新魏", 25, 75));
    StepLabel->setStyleSheet("color:brown");
    LevelLabel=new QLabel(this);
    LevelLabel->move(300,610);
    LevelLabel->setFont(QFont("华文新魏", 25, 75));
    LevelLabel->setStyleSheet("color:brown");
    //关联计时器
    tim=new QTimer(this);
    connect(tim,&QTimer::timeout,[=](){
        --time;
        TimeLabel->setText(QString("%1").arg(time,2));
        if(!time)
            endscene->exec();
    });

}

void PipeRepairer::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,this->width(),this->height(),QPixmap(":/xiushuiguan/background5.png"));
}

void PipeRepairer::mousePressEvent(QMouseEvent *e )
{
    int x = e->x(), y = e->y();
    if(!wait&&x>=500 && x<=1050 && y>=50 && y<=700)//限制点击范围
    {

        //计算相对位置
        x=(x-500)/50;
        y=(y-50)/50;
        if(pipes[x][y].change())
        {
            --steps;
            StepLabel->setText(QString("%1").arg(steps,2));
            if(!steps)
            {
                wait=1;
                tim->stop();
                //失败界面
                qDebug()<<"失败";
                endscene->exec();
            }
        }
        for(auto& i:Fill)
            i.first->UP();
        Fill.clear();
        bool ok1=success(sx,sy,sd);
        for(auto& i:Fill)
            i.first->fill(i.second%2,i.second/2);
        if(ok1)
        {
            tim->stop();
            wait=1;
            qDebug()<<"胜利";
            //这里不要胜利画面了因为太简单了 所以要快！ 最优决策
            //添加一个胜利音效即可
            QTimer::singleShot(500,[=](){
                random();
            });
        }
    }
}
//直水管0 十字水管1 单个弯水管2 两个弯水管3 无水管4 尾水管5 头水管6
//  直水管可以旋转2次 十字水管0次 单个弯水管4次 两个弯水管2次 无水管0次 尾水管0次 头水管0次

enum Direct{up,right,down,left};
int ret;
void PipeRepairer::random()
{
    //游戏的开始  时间越来越少
    ++level;
    time=21-level;
    steps=20;
    LevelLabel->setText(QString("%1").arg(level,2));
    StepLabel->setText(QString("%1").arg(steps,2));
    TimeLabel->setText(QString("%1").arg(time,2));
    Fill.clear();

    //上右下左 0 1 2 3
    memset(vis,0,sizeof(vis));
    steps=20;
    //随机生成一条通路
    //随机出边界
    //0左右边界  1上下边界  且默认入水口在左上位置 出水口在右下位置
    side=rand(1);
    //调用生成函数
    qDebug()<<choscene->choice;
    if(choscene->choice)
        hard_level();
    else
        easy_level();
    //起点终点的设置
    pipes[start.x()][start.y()].set(5,side?Direct::down:Direct::right);
    pipes[end.x()][end.y()].set(6,side?Direct::up:Direct::left);
    vis[start.x()][start.y()]=1;
    vis[end.x()][end.y()]=1;
    //方便判断success函数的调用
    sd=pipes[start.x()][start.y()].status;
    sx=start.x()+dir[sd][0],sy=start.y()+dir[sd][1];
    //没有的补全，有的随机一下自己的状态
    for(int i=0;i<11;++i)
        for(int j=0;j<13;++j)
        {
            if(!vis[i][j])
                pipes[i][j].set(rand(4));
            if(pipes[i][j].upp>1)
                pipes[i][j].status=rand(pipes[i][j].upp-1);
            else if(pipes[i][j].upp==1)
                pipes[i][j].status=0;
            pipes[i][j].UP();
            pipes[i][j].show();
        }
    //开始玩起来
    wait=0;
    //重新计时
    tim->start(1000);
    //防止随机失败
    bool ok=success(sx,sy,sd);
    for(auto& i:Fill)
        i.first->fill(i.second%2,i.second/2);
    if(ok)
        random();
}

void PipeRepairer::easy_level()
{
    //确定边界后 随机起点和终点的具体点位
    int pos;
    if(side)
    {
        start.setY(0),start.setX(rand(10));
        end.setY(12);end.setX(rand(10));
        if(start.x()!=end.x())
        {
            //找出断点
            pos=rand(11,1);
            //开头部分
            for(int i=1;i<pos;++i)
            {
                vis[start.x()][i]=1;
                pipes[start.x()][i].set(rand(1));
            }
            int mn=start.x()<end.x()?start.x():end.x();
            int mx=mn==start.x()?end.x():start.x();
            //拐角
            vis[end.x()][pos]=vis[start.x()][pos]=1;
            pipes[start.x()][pos].set(rand(3,2));
            //中间部分
            for(int i=mn+1;i<mx;++i)
            {
                vis[i][pos]=1;
                pipes[i][pos].set(1);
            }
            //结束部分
            pipes[end.x()][pos].set(rand(3,2));
            for(int i=pos+1;i<12;++i)
            {
                vis[end.x()][i]=1;
                pipes[end.x()][i].set(rand(1));
            }

        }
        else
            //坐标相同的情况
            for(int i=1;i<12;++i)
            {
                vis[start.x()][i]=1;
                pipes[start.x()][i].set(rand(1));
            }
    }
    else//同理
    {
        //左右
        start.setX(0),start.setY(rand(12));
        end.setX(10);end.setY(rand(12));
        if(start.y()!=end.y())
        {
            pos=rand(9,1);
            for(int i=1;i<pos;++i)
            {
                vis[i][start.y()]=1;
                pipes[i][start.y()].set(rand(1));
            }
            int mn=start.y()<end.y()?start.y():end.y();
            int mx=mn==start.y()?end.y():start.y();
            vis[pos][start.y()]=vis[pos][end.y()]=1;
            pipes[pos][start.y()].set(rand(3,2));
            for(int i=mn+1;i<mx;++i)
            {
                vis[pos][i]=1;
                pipes[pos][i].set(rand(1));
            }
            pipes[pos][end.y()].set(rand(3,2));
            for(int i=pos+1;i<10;++i)
            {
                vis[i][end.y()]=1;
                pipes[i][end.y()].set(rand(1));
            }

        }
        else
            for(int i=1;i<10;++i)
            {
                vis[i][start.y()]=1;
                pipes[i][start.y()].set(rand(1));
            }
    }
}

void PipeRepairer::hard_level()
{
    if(side)
    {
        start.setY(0),start.setX(rand(10));
        end.setY(12);end.setX(rand(10));
        ban=Direct::up;
        //关于上下的方向数组
    }
    else//同理
    {
        //左右
        start.setX(0),start.setY(rand(12));
        end.setX(10);end.setY(rand(12));
        ban=Direct::left;
    }
    ret=rand(0)?1:-1;
    //从起点开始后一个点开始dfs
    dfs(start.x()+(!side),start.y()+side,side?Direct::down:Direct::right);
}

bool PipeRepairer::dfs(int x, int y, int last)
{
    if(x==end.x()&&y==end.y())
        return last==(side?Direct::down:Direct::right);
    if(!legal(x,y)||vis[x][y])
        return false;
    vis[x][y]=1;
    bool used[4]={0};
    used[ban]=used[last<2?last+2:last-2]=1;//ban掉的方向和相反方向不走
    int tx,ty;;
    for(int i=ban,c=0;c<4;i=(i+ret+4)%4,++c)
    {
        if(used[i]) continue;
        tx=x+dir[i][0],ty=y+dir[i][1];
        if(dfs(tx,ty,i))
        {
            if(i==last)
                pipes[x][y].set(rand(1));
            else
                pipes[x][y].set(rand(3,2));
            return true;
        }
    }
    vis[x][y]=0;
    return false;
}

bool PipeRepairer::success(int x,int y,int last)
{
    //检测是否越界
    if(!legal(x,y))
        return false;
    //检测是否已末尾需要的角度给予水
    if(x==end.x()&&y==end.y()&&abs(last-pipes[end.x()][end.y()].status)==2)
        return true;
    //根据种类以及进入的方向确定出去的方向
    switch(pipes[x][y].type)
    {
    case 0:
        if((pipes[x][y].status&&(last==Direct::up||last==Direct::down))||(!pipes[x][y].status&&(last==Direct::left||last==Direct::right)))
        {
            Fill.push_back(P(&pipes[x][y],0));
            return success(x+dir[last][0],y+dir[last][1],last);
        }
        else
            return false;
    case 1:
    {
        if(last==Direct::up||last==Direct::down)
            Fill.push_back(P(&pipes[x][y],3));
        else
            Fill.push_back(P(&pipes[x][y],2));
        return success(x+dir[last][0],y+dir[last][1],last);
    }
    case 2:
        switch(pipes[x][y].status)
        {
        case 0:
            if(last==Direct::right||last==Direct::down)
                Fill.push_back(P(&pipes[x][y],0));
            if(last==Direct::right)
                return success(x+dir[Direct::up][0],y+dir[Direct::up][1],Direct::up);
            else if(last==Direct::down)
                return success(x+dir[Direct::left][0],y+dir[Direct::left][1],Direct::left);
            else
                return false;
        case 1:
            if(last==Direct::left||last==Direct::down)
                Fill.push_back(P(&pipes[x][y],0));
            if(last==Direct::down)
                return success(x+dir[Direct::right][0],y+dir[Direct::right][1],Direct::right);
            else if(last==Direct::left)
                return success(x+dir[Direct::up][0],y+dir[Direct::up][1],Direct::up);
            else
                return false;
        case 2:
            if(last==Direct::left||last==Direct::up)
                Fill.push_back(P(&pipes[x][y],0));
            if(last==Direct::up)
                return success(x+dir[Direct::right][0],y+dir[Direct::right][1],Direct::right);
            else if(last==Direct::left)
                return success(x+dir[Direct::down][0],y+dir[Direct::down][1],Direct::down);
            else
                return false;
        case 3:
            if(last==Direct::right||last==Direct::up)
                Fill.push_back(P(&pipes[x][y],0));
            if(last==Direct::right)
                return success(x+dir[Direct::down][0],y+dir[Direct::down][1],Direct::down);
            else if(last==Direct::up)
                return success(x+dir[Direct::left][0],y+dir[Direct::left][1],Direct::left);
            else
                return false;
        }
    case 3:
        if(pipes[x][y].status)
        {
            if(last==Direct::up||last==Direct::right)
                Fill.push_back(P(&pipes[x][y],3));
            else
                Fill.push_back(P(&pipes[x][y],2));
            switch(last)
            {
            case Direct::up:
                return success(x+dir[Direct::left][0],y+dir[Direct::left][1],Direct::left);
            case Direct::right:
                return success(x+dir[Direct::down][0],y+dir[Direct::down][1],Direct::down);
            case Direct::down:
                return success(x+dir[Direct::right][0],y+dir[Direct::right][1],Direct::right);
            case Direct::left:
                return success(x+dir[Direct::up][0],y+dir[Direct::up][1],Direct::up);
            }
        }
        else
        {
            if(last==Direct::up||last==Direct::left)
                Fill.push_back(P(&pipes[x][y],3));
            else
                Fill.push_back(P(&pipes[x][y],2));
            switch(last)
            {
            case Direct::up:
                return success(x+dir[Direct::right][0],y+dir[Direct::right][1],Direct::right);
            case Direct::right:
                return success(x+dir[Direct::up][0],y+dir[Direct::up][1],Direct::up);
            case Direct::down:
                return success(x+dir[Direct::left][0],y+dir[Direct::left][1],Direct::left);
            case Direct::left:
                return success(x+dir[Direct::down][0],y+dir[Direct::down][1],Direct::down);
            }
        }
    }
    return false;
}

bool PipeRepairer::legal(int x,int y)
{
    return x>=0&&y>=0&&x<11&&y<13;
}
//随机生成一个[low,up]的数
int PipeRepairer::rand(int up,int low)
{
    return qrand()%(up-low+1)+low;
}


QPoint PipeRepairer::transform(int x, int y)
{
    return QPoint(x*50+500,y*50+50);
}

void PipeRepairer::stopVideo()
{
    player->pause();
}

void PipeRepairer::continueVideo()
{
    player->play();
}

void PipeRepairer::muteVideo()
{

    player->setMuted(muted);
    muted=!muted;
}

PipeRepairer::~PipeRepairer()
{
    delete endscene;
    delete ui;
}
