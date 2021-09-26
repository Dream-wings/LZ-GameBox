#ifndef FIRSTORSCENDSCENE_H
#define FIRSTORSCENDSCENE_H
#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <mybtn.h>
class FirstOrScendScene :public QWidget
{
public:
    FirstOrScendScene(QWidget *parent = nullptr);
    void  paintEvent(QPaintEvent*);
    Mybtn* firstbtn,*secondbtn;
};

#endif // FIRSTORSCENDSCENE_H
