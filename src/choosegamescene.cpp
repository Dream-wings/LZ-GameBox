#include "choosegamescene.h"

ChooseGameScene::ChooseGameScene(QWidget *parent) : QWidget(parent)
{
    setFixedSize(1080,720);
    setWindowTitle("选择游戏");
    setWindowIcon(QIcon(":/choosescene/mainicon.png"));
    //按钮的设置
    huangjingkuanggbtn = new Mybtn(":/choosescene/huangjinkuanggongicon.png",":/choosescene/huangjinkuanggongicon_press.png",0.2);
    chessbtn = new Mybtn(":/choosescene/wuziqiicon.png",":/choosescene/wuziqiicon_press.png",0.2);
    xiushuiguanbtn = new Mybtn(":/choosescene/xiushuiguanicon.png",":/choosescene/xiushuiguanicon_press.png",0.2);
    backbtn=new Mybtn(":/picture_for_chess/button4_exit.png",":/picture_for_chess/button4_exit_press.png",0.14);
    huangjingkuanggbtn->setParent(this);
    chessbtn->setParent(this);
    xiushuiguanbtn->setParent(this);
    backbtn->setParent(this);

    huangjingkuanggbtn->move(340,450);
    chessbtn->move(470,450);
    xiushuiguanbtn->move(600,450);
    backbtn->move(1020,650);

    //创建游戏空间
    wuziqi = new Wuziqi;
    piperepairer = new PipeRepairer;
    //黄金矿工游戏
    connect(huangjingkuanggbtn,&Mybtn::clicked,[=](){
        goldwidget = new Goldwidget;
        connect(goldwidget,&Goldwidget::back,[=](){
            goldwidget->close();
            delete goldwidget;
            show();
        });
        goldwidget->show();
        hide();
    });
    //五子棋游戏
    connect(chessbtn,&Mybtn::clicked,[=](){
        wuziqi->restart(-1);
        wuziqi->show();
        hide();
    });
    connect(wuziqi,&Wuziqi::back,[=](){
        show();
    });
    //修水管游戏
    connect(piperepairer->choscene->hardbtn,&QPushButton::clicked,[=](){
        piperepairer->choscene->choice=1;
        piperepairer->choscene->hide();
    });
    connect(piperepairer->choscene->easybtn,&QPushButton::clicked,[=](){
        piperepairer->choscene->choice=0;
        piperepairer->choscene->hide();
    });
    connect(xiushuiguanbtn,&Mybtn::clicked,[=](){
        piperepairer->show();
        piperepairer->choscene->exec();
        piperepairer->random();
        piperepairer->level=0;
        piperepairer->player->setPlaylist(piperepairer->medialist);
        piperepairer->player->play();
        hide();
    });
    connect(piperepairer,&PipeRepairer::back,[=](){
        show();
    });

    //返回上一级界面
    connect(backbtn,&Mybtn::clicked,[=](){
        emit back();
        hide();
    });

}

ChooseGameScene::~ChooseGameScene()
{
    delete piperepairer;
    delete goldwidget;
    delete wuziqi;
}

void ChooseGameScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,this->width(),this->height(),QPixmap(":/choosescene/choose4.png"));
}

