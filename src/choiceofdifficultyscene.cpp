#include "choiceofdifficultyscene.h"

ChoiceOfDifficultyScene::ChoiceOfDifficultyScene(QWidget* parent):QDialog(parent)
{
    setFixedSize(600,600);
    setWindowTitle("选择游戏难度");
    setWindowIcon(QIcon(":/xiushuiguan/icon.jpg"));
    easybtn=new Mybtn(":/xiushuiguan/easybtn.png",":/xiushuiguan/easybtn_press.png",0.9);
    hardbtn=new Mybtn(":/xiushuiguan/hardbtn.png",":/xiushuiguan/hardbtn_press.png",0.9);
    easybtn->setParent(this);
    hardbtn->setParent(this);
    easybtn->move(180,290);
    hardbtn->move(180,390);
}
void ChoiceOfDifficultyScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,this->width(),this->height(),QPixmap(":/xiushuiguan/choiceofdifficulty.png"));
}
