#include "registerscene.h"
#include <QDebug>
#include <QTimer>
#include <QMessageBox>

RegisterScene::RegisterScene()
{
    setFixedSize(400,300);//窗口大小
    setWindowIcon(QIcon(":/choosescene/mainicon.png"));
    setWindowTitle("注册窗口");

    rigisteR_userNameLbl->setText("账户");
    rigisteR_userNameLbl->move(80,95);
    rigisteR_userNameLbl->setStyleSheet("color:white;");

    rigisteR_userNameLEd->setPlaceholderText(tr("请输入账户:"));
    rigisteR_userNameLEd->setMaxLength(5);
    rigisteR_userNameLEd->move(130,95);


    rigisteR_pwdLbl->setText("密码");
    rigisteR_pwdLbl->move(80,135);
    rigisteR_pwdLbl->setStyleSheet("color:white;");

    rigisteR_pwdLEd->setPlaceholderText(tr("请输入密码:"));
    rigisteR_pwdLEd->move(130,135);
    rigisteR_pwdLEd->setEchoMode(QLineEdit::Password);//输入的密码以*显示

    rigisteR_pwdLbl_2->setText("确认密码");
    rigisteR_pwdLbl_2->move(50,175);
    rigisteR_pwdLbl_2->setStyleSheet("color:white;");

    rigisteR_pwdLEd_2->setPlaceholderText(tr("请再次输入密码:"));
    //rigisteR_pwdLEd_2->setFixedSize(280,30);
    rigisteR_pwdLEd_2->move(130,175);
    rigisteR_pwdLEd_2->setEchoMode(QLineEdit::Password);

    yesbtn=new Mybtn(":/res/release2.png",":/res/press2.png",0.5);
    yesbtn->setParent(this);
    yesbtn->move(100,250);

    BackButton=new Mybtn(":/res/release3.png",":/res/press3.png",0.5);
    connect(BackButton,&QPushButton::clicked,[=](){
        QTimer::singleShot(200,[=](){
            emit back();
        });
    });
    BackButton->setParent(this);
    BackButton->move(200,250);

    connect(yesbtn,&Mybtn::clicked,[=](){
        if(check_acc(rigisteR_userNameLEd->text()))
        {
            if(rigisteR_pwdLEd->text()==rigisteR_pwdLEd_2->text())//注册成功弹出success对话框，3秒后返回登录界面;注册失败弹出fail对话框。
            {
                QFile file("pwd.txt");
                file.open(QIODevice::Append);
                QTextStream out(&file);
                out<<rigisteR_userNameLEd->text().toUtf8().constData()<<" "<<rigisteR_pwdLEd->text().toUtf8().constData()<<endl;
                file.close();
                Register_success_scene *success=new Register_success_scene;
                hide();
                success->show();
                QTimer::singleShot(3000,[=](){
                    success->close();
                    emit back();
                    delete success;
                });
            }
            else
            {
                Register_fail_scene *fail=new Register_fail_scene;
                fail->show();
                QTimer::singleShot(2000,[=](){
                    fail->close();
                    rigisteR_userNameLEd->clear();
                    rigisteR_pwdLEd->clear();
                    rigisteR_pwdLEd_2->clear();
                    delete fail;
                });
            }
        }
        else
        {
            rigisteR_userNameLEd->clear();
            rigisteR_pwdLEd->clear();
            rigisteR_pwdLEd_2->clear();
        }


    });

}

bool RegisterScene::check_acc(QString a)

{
    int b=a.length();
    if(b<=3)//账号数字小于4警告
    {
        QMessageBox::warning(this,"警告","账号必须大于等于四位！");
        return false;
    }
    for(auto i:a)
    {
        if(!i.isDigit())//如果不全是数字警告
        {
            QMessageBox::warning(this,"警告","账号只能由全数字构成！");
            return false;
        }
    }

    return true;
}
RegisterScene::~RegisterScene()
{

}
void  RegisterScene::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QPixmap pix;
    bool ret=pix.load(":/res/FailToLoginScene.jpg");
    if(!ret)
        qDebug()<<"图片加载出现错误"<<endl;
    else
        painter.drawPixmap(0,0,this->width(),this->height(),pix);
}
