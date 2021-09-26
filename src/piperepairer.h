#ifndef PIPEREPAIRER_H
#define PIPEREPAIRER_H
#include <QMainWindow>
#include <QPaintEvent>
#include <QPainter>
#include <QVector>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QMediaPlaylist>
#include <QTimer>
#include <QVector>
#include "piperepairerendscene.h"
#include <choiceofdifficultyscene.h>
#include "pipe.h"

namespace Ui {
class PipeRepairer;
}

typedef  QPair<Pipe*,int> P;

class PipeRepairer : public QMainWindow
{
    Q_OBJECT

public:
    QMediaPlayer *player;
    QMediaPlaylist *medialist;
    bool wait;
    int level;
    ChoiceOfDifficultyScene* choscene;

    PipeRepairer(QMainWindow *parent = nullptr);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent* e);
    ~PipeRepairer();
    void random();


private:
    Ui::PipeRepairer *ui;
    int sx,sy,sd;
    int steps;
    int time;
    int ban;//创建时通路时ban掉的方向
    //随机的边以及是否处于等待状态
    bool side;
    bool vis[11][13];//标记是否存在vis  设计关卡时使用
    bool muted=true;
    QLabel *TimeLabel;
    QLabel *StepLabel;
    QLabel *LevelLabel;
    QTimer *tim;
    QVector<P> Fill;
    //三种按钮
    Mybtn* stopbtn;
    Mybtn* continuebtn;
    Mybtn* mutebtn;
    Pipe pipes[11][13];//存储地图上的水管
    //失败界面
    PipeRepairerEndScene* endscene;

    //播放视频的变量
    QVideoWidget *videoWidget;
    //一条通路起点和终点
    QPoint start,end;


    bool success(int x,int y,int last);
    void easy_level();
    void hard_level();
    bool dfs(int x,int y,int last);
    int rand(int up,int low=0);
    bool legal(int x,int y);
    QPoint transform(int x,int y);

signals:
    void back();


public slots:
    void continueVideo();
    void stopVideo();
    void muteVideo();
};

#endif // PIPEREPAIRER_H
