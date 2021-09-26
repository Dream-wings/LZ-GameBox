#ifndef REGISTERSCENE_H
#define REGISTERSCENE_H
#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QtDebug>
#include "mybtn.h"
#include "register_success_scene.h"
#include "register_fail_scene.h"
#include <QLabel>
#include <QLineEdit>
class RegisterScene : public QWidget
{
    Q_OBJECT
public:
    RegisterScene();
    ~RegisterScene();
    void  paintEvent(QPaintEvent*);
private:
    QLabel *rigisteR_userNameLbl=new QLabel(this);//账户
    QLineEdit *rigisteR_userNameLEd=new QLineEdit(this);//账户输入框
    QLabel *rigisteR_pwdLbl=new QLabel(this);//密码
    QLineEdit *rigisteR_pwdLEd=new QLineEdit(this);//密码输入框
    QLabel *rigisteR_pwdLbl_2=new QLabel(this);//再次输入密码
    QLineEdit *rigisteR_pwdLEd_2=new QLineEdit(this);//再次密码输入框
    Mybtn *yesbtn;
    Mybtn *BackButton;

    bool check_acc(QString a);
signals:
    void back();
};

#endif // REGISTERSCENE_H
