#include "startwidget.h"
#include "ui_startwidget.h"
#include "mybtn.h"

#include <QDebug>
#include <QPainter>
#include <QTimer>

Startwidget::Startwidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Startwidget)
{
    ui->setupUi(this);

    //窗口初始化
    setFixedSize(1080,720);
    setWindowIcon(QIcon(":/choosescene/mainicon.png"));
    setWindowTitle("小游戏盒");

    //开始按钮设置
    StartBtn=new Mybtn(":/res/StartBtn.png");
    StartBtn->setParent(this);
    StartBtn->move((width()-StartBtn->width())/2,450);
    mainscene=new MainScene;
    connect(mainscene,&MainScene::back,[=](){
        mainscene->close();
        show();
        player->setMedia(QUrl("qrc:/res/backmus.mp3"));
        player->play();
    });
    connect(StartBtn,&QPushButton::clicked,[=](){
        StartBtn->zoom();
        QTimer::singleShot(300,[=](){
            hide();
            player->stop();
            mainscene->show();
        });
    });

    //设置背景音乐相关
    player=new QMediaPlayer(this);
    player->setMedia(QUrl("qrc:/res/backmus.mp3"));
    player->setVolume(20);
    player->play();
    MuteBtn=new Mybtn(":/xiushuiguan/mutebtn.png",":/xiushuiguan/mutebtn.png",0.1);
    MuteBtn->setParent(this);
    MuteBtn->move(15,height()-40);
    connect(MuteBtn,&QPushButton::clicked,[=](){
        player->setMuted(!player->isMuted());
    });
}

void Startwidget::paintEvent(QPaintEvent*)
{
    QPainter paint(this);
    QPixmap pix;
    bool ret=pix.load(":/res/Startbg.png");
    if(!ret)
        qDebug()<<"开始界面图片加载出现错误";
    paint.drawPixmap(0,0,pix);
}

Startwidget::~Startwidget()
{
    delete mainscene;
    delete ui;
}

