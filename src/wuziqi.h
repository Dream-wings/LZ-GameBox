#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <chess.h>
#include <QVector>
#include <QList>
#include <QUdpSocket>
#include <QMediaPlayer>
#include <chessendscene.h>
#include <QTimer>
#include <firstorscendscene.h>
#include <connectinput.h>

class AIsteps
{
public:
    AIsteps(int xx,int yy);
    AIsteps(int xx=0);
    int pos;
    int x;
    int y;
    bool operator<(const AIsteps& a)const;
};

class Wuziqi : public QWidget
{
    Q_OBJECT

public:
    static QPoint transform(int x,int y);
    Wuziqi(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent* e);
    void mousePressEvent(QMouseEvent* e);
    void solve();//对网络数据进行处理
    QUdpSocket* udp;
    //测试
    int PORT[2]={2222,6666};
    QString IP[2]={"127.0.0.1","127.0.0.1"};
    ConnectInput* select_person;

    void restart(int x);//棋盘清空

private:
    int type;
    ChessEndScene *ces;
    QList<AIsteps> steps;
    QVector<Chess*> vec;
    QPixmap head[5];
    QLabel *TimeRecorder;
    QLabel *ScoreRecorder;
    QLabel *people1;
    QLabel *people2;
    QLabel *zhaozi;
    QMediaPlayer* player;
    ConnectBtn *readybtn;
    Mybtn *returnbtn;
    Mybtn *regretbtn;
    Mybtn *giveupbtn;
    Mybtn *AIbtn;
    Mybtn *Onlinebtn;
    FirstOrScendScene* select_computer;
    bool playing;
    bool AImode;
    int vis[361][361];
    int rank[2];//记二人的分
    int tim[2];//记录时间
    int ste;//记录总共下了多少步棋
    bool wait;//判断是否可以行动
    bool ready[2];//是否已准备 0自己 1别人
    bool bind[2];//是否开始绑定
    bool conti[2];//双方的继续信号
    bool first;//判断是否是第一次进入界面
    bool doing;
    QTimer* time;//步数计时器

    void startgame();
    void init();
    int set_head();
    bool check(int x,int y);//判断这步棋是否会获得胜利中的判断函数
    bool legal(int x,int y); //判断是否越界
    void AI();//人工模式
    void success();//胜利操作
    void regret(bool x);//悔棋操作 真悔2步  假悔1步
    void send(QByteArray t);
    void send(int x);
    bool Erase(int x);

signals:
    void back();
};


#endif // WIDGET_H
