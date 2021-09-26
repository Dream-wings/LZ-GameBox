#ifndef GOLDWIDGET_H
#define GOLDWIDGET_H
#include "grab.h"
#include "obj.h"
#include <QWidget>
#include <QTimer>
#include <QMouseEvent>
#include <QLabel>
#include <mybtn.h>

class Goldwidget : public QWidget
{
    Q_OBJECT
private:
    Grab* g;
    int time,dir,count,ret;//已游戏时间，和当前出勾方向,出勾时间，是否返回
    int speed,level,grades,suspend,last;  //代表出勾速度 关卡数 分数 记录出勾的时间
    bool shooting; //表示是否当前处于出勾状态,表示是否勾中
    Obj *target; //代表当前勾中的目标 未勾中设置为nullptr
    QTimer* st,*whole;//游戏时间计时器
    QVector<Obj*> mine[11],tmp;  //存储每个斜率对应的矿石  当前关卡中的所有矿石
    QLabel *TimeLabel,*GradesLabel,*LevelLabel,*TargetLabel,success,lose;
    Mybtn* backbtn;

public:
    explicit Goldwidget(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent *event);
    void initial();
    void swing(int msec);
    void load();

signals:
    void back();

};

#endif // GOLDWIDGET_H
