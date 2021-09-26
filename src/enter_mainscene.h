#ifndef MAINSCENE_H
#define MAINSCENE_H
#include <QMainWindow>
#include <QPaintEvent>
#include <QLabel>
#include <QLineEdit>
#include "failtologinScene.h"
#include "choosegamescene.h"
#include "registerscene.h"
#include "failtologinScene.h"
#include "mybtn.h"
#include "sliderbutton.h"


class MainScene : public QMainWindow
{
    Q_OBJECT

public:
    MainScene(QWidget *parent = nullptr);
    ~MainScene();
    ChooseGameScene* choosegamescene;
    void  paintEvent(QPaintEvent*);
private:
    FailToLogin *fail;
    RegisterScene* registerscene;
    QLabel *userNameLbl;         //"用户名"标签
    QLineEdit *userNameLEd;      //用户名编辑行
    QLabel *pwdLbl;              //"密码"标签
    QLineEdit *pwdLEd;           //密码编辑行
    Mybtn *LoginButton;          //登录按钮
    Mybtn *RegisterButton;       //注册按钮
    Mybtn *BackButton;         //返回按钮
    SliderButton * test;
signals:
    void back();
};
#endif // MAINSCENE_H
