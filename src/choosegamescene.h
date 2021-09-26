#ifndef CHOOSEGAMESCENE_H
#define CHOOSEGAMESCENE_H

#include "mybtn.h"
#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include "goldwidget.h"
#include "wuziqi.h"
#include "piperepairer.h"

class ChooseGameScene : public QWidget
{
    Q_OBJECT
public:
    explicit ChooseGameScene(QWidget *parent = nullptr);
    ~ChooseGameScene();

    Goldwidget* goldwidget;
    Wuziqi* wuziqi;
    PipeRepairer* piperepairer;
    void paintEvent(QPaintEvent *event);
private:
    Mybtn *huangjingkuanggbtn;
    Mybtn *chessbtn;
    Mybtn *xiushuiguanbtn;
    Mybtn *backbtn;
signals:
    void back();

};

#endif // CHOOSEGAMESCENE_H
