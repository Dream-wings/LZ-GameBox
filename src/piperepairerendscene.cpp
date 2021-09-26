#include "piperepairerendscene.h"
#include <QIcon>
PipeRepairerEndScene::PipeRepairerEndScene()
{

    setWindowIcon(QIcon(":/xiushuiguan/icon.jpg"));
    setFixedSize(600,600);
    setWindowTitle("失败！");
    restartbtn = new Mybtn(":/xiushuiguan/button6_restart.png",":/xiushuiguan/button6_restart_press.png",0.5);
    exitbtn = new Mybtn(":/xiushuiguan/button6_exit.png",":/xiushuiguan/button6_exit_press.png",0.5);
    restartbtn->setParent(this);
    exitbtn->setParent(this);
    restartbtn->move(140,420);
    exitbtn->move(320,420);
}
void PipeRepairerEndScene::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,this->width(),this->height(),QPixmap(":/xiushuiguan/background4.png"));
}
