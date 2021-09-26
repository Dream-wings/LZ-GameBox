#include "enter_mainscene.h"
#include <QPainter>
#include <QDebug>
#include <QFileInfo>
#include <QFile>
#include <QDialog>
#include <QTimer>
#include <QNetworkInterface>

QString getHostIpAddress()
{
    QString strIpAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // 获取第一个本主机的IPv4地址
    int nListSize = ipAddressesList.size();
    for (int i = 0; i < nListSize; ++i)
    {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                ipAddressesList.at(i).toIPv4Address()) {
            strIpAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // 如果没有找到，则以本地IP地址为IP
    if (strIpAddress.isEmpty())
        strIpAddress = QHostAddress(QHostAddress::LocalHost).toString();
    return strIpAddress;
}

MainScene::MainScene(QWidget *parent): QMainWindow(parent)
{
    //设置主背景
    setFixedSize(1080,720);//设置固定大小
    setWindowTitle("登录窗口");//设置窗口标题
    setWindowIcon(QIcon(":/choosescene/mainicon.png"));

    //登录界面具体实现
    userNameLbl = new QLabel(this);   //new一个标签对象
    userNameLEd = new QLineEdit(this);
    userNameLbl->move(350,340);         //移动位置
    userNameLbl->setText("账户:");
    userNameLEd->move(420,340);
    userNameLEd->setMaxLength(5);
    userNameLEd->setPlaceholderText(tr("请输入账户:"));
    userNameLEd->setFixedSize(280,30);
    pwdLbl = new QLabel(this);
    pwdLbl->move(350,390);
    pwdLbl->setText("密码:");
    pwdLEd = new QLineEdit(this);
    pwdLEd->move(420,390);
    pwdLEd->setPlaceholderText("请输入密码!");
    pwdLEd->setEchoMode(QLineEdit::Password);//输入的密码以*显示
    pwdLEd->setFixedSize(280,30);
    //登录按钮
    LoginButton=new Mybtn(":/res/login_in1.png",":/res/press1.png");
    LoginButton->setParent(this);
    LoginButton->move(270,495);

    choosegamescene=new ChooseGameScene;
    test=new SliderButton(this);
    test->move(10,height()-40);

    connect(LoginButton,&Mybtn::clicked,[=](){
        static int counter=0;
        qDebug()<<"点击了Login in按钮"<<endl;
        QString text1[2] = {userNameLEd->text() , pwdLEd->text()};//读取用户输入的账户密码数据
        //与本地文件进行对比，相同则进入游戏界面，否则弹出对话框要求重新输入。错误三次以后自动关闭游戏。
        //读取本地文件
        QFile file;
        QFileInfo info("pwd.txt");
        QString text2[2];
        bool ok=0;
        if(info.exists())
        {
            file.setFileName("pwd.txt");
            file.open(QIODevice::ReadOnly);
            QTextStream in(&file);
            while(!in.atEnd())
            {
                in>>text2[0]>>text2[1];
                ok=(text1[0]==text2[0].trimmed()&&text1[1]==text2[1].trimmed()&&text1[0]!=""&&text1[1]!="");
                if(ok)
                    break;
            }
            file.close();
        }
        if(ok)
        {
            //登陆成功，进入游戏界面
            qDebug()<<"登陆成功";
            this->close();
            choosegamescene->wuziqi->udp=new QUdpSocket(choosegamescene->wuziqi);
            //读入窗口
            choosegamescene->wuziqi->PORT[0]=text1[0].toInt();
            //设置端口
            choosegamescene->wuziqi->select_person->my_port_s=QString::number(choosegamescene->wuziqi->PORT[0]);
            //设置IP
            if(test->m_button_status)
                choosegamescene->wuziqi->IP[0]="127.0.0.1";
            else
                choosegamescene->wuziqi->IP[0]=getHostIpAddress();
            choosegamescene->wuziqi->select_person->my_IP_s=choosegamescene->wuziqi->IP[0];
            //更新五子棋界面的显示
            choosegamescene->wuziqi->select_person->myupdate();
            //绑定IP与端口
            choosegamescene->wuziqi->udp->bind(QHostAddress( choosegamescene->wuziqi->IP[0]), choosegamescene->wuziqi->PORT[0],QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);
            //连接接受信号
            connect(choosegamescene->wuziqi->udp,&QUdpSocket::readyRead,choosegamescene->wuziqi,&Wuziqi::solve);
            qDebug()<<"登陆界面"<<choosegamescene->wuziqi->select_person->my_IP_s<<choosegamescene->wuziqi->select_person->my_port_s;


            choosegamescene->show();
            connect(choosegamescene,&ChooseGameScene::back,[=](){
                delete choosegamescene->wuziqi->udp;
                choosegamescene->close();
                show();
                userNameLEd->clear();
                pwdLEd->clear();
            });
        }
        else
        {
            //弹出错误窗口并提示重新输入
            counter++;
            fail=new FailToLogin;
            pwdLEd->clear();//密码槽清空
            fail->exec();
            if(counter>3)
                close();
            delete fail;
        }
    });

    RegisterButton=new Mybtn(":/res/release2.png",":/res/press2.png");
    connect(RegisterButton,&QPushButton::clicked,[=](){
        registerscene =new RegisterScene;
        connect(registerscene,&RegisterScene::back,[=](){
            registerscene->close();
            userNameLEd->clear();
            pwdLEd->clear();
            show();
        });
        close();
        registerscene->show();
    });
    RegisterButton->setParent(this);
    RegisterButton->move(470,495);


    BackButton=new Mybtn(":/res/release3.png",":/res/press3.png");
    connect(BackButton,&QPushButton::clicked,[=](){
        QTimer::singleShot(200,[=](){
            emit back();
        });
    });
    BackButton->setParent(this);
    BackButton->move(670,495);

}

void  MainScene::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QPixmap pix;
    bool ret=pix.load(":/res/Startbg2.png");
    if(!ret)
        qDebug()<<"图片加载出现错误"<<endl;
    else
        painter.drawPixmap(0,0,this->width(),this->height(),pix);
}

MainScene::~MainScene()
{
    delete choosegamescene;
}
