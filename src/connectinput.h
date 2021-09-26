#ifndef CONNECTINPUT_H
#define CONNECTINPUT_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <mybtn.h>

class ConnectBtn:public Mybtn
{
public:
    explicit ConnectBtn(QString a,QString b,QString c,double ratio=1);
    bool change;
    void init();
private:
    QString conpath;
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
};

class ConnectInput : public QWidget
{
    Q_OBJECT
public:
    QLineEdit* friend_IP_w;
    QLineEdit* friend_port_w;
    QString my_IP_s;
    QString my_port_s;
    ConnectBtn* bindbtn;


    void myupdate();
    void paintEvent(QPaintEvent* e);
    void set(QString a);
    explicit ConnectInput(QWidget *parent = nullptr);
private:
    QLabel* friend_IP;
    QLabel* friend_port;
    QLabel* my_IP;
    QLabel* my_port;

signals:

};

#endif // CONNECTINPUT_H
