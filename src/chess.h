#ifndef ITEM_H
#define ITEM_H
#include <QPoint>
#include <QWidget>
#include <QLabel>
class Chess: public QLabel
{
private:


public:
    explicit Chess(int x=0,int y=0,bool col=0,QWidget *parent =0);
    static const QString cpath[3]; //0 表示黑 1表示白棋子图片 2表示音效图片
    QPoint pos;//棋子位置

};

#endif // ITEM_H
