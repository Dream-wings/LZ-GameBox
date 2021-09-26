#ifndef PIPE_H
#define PIPE_H
#include <QPoint>
#include <QMainWindow>
#include <QLabel>
class Pipe : public QLabel
{
    Q_OBJECT
public:
    int type;
    int status;//status代表各种水管的状态
    int upp;//旋转上限

    explicit Pipe();//i代表种类,s代表状态,x,y代表相对位置坐标
    void set(int i,int s=0);
    void fill(int x,int c=1);
    bool change();
    void UP();
};

#endif // PIPE_H
