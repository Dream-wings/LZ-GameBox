#ifndef OBJ_H
#define OBJ_H

#include <QLabel>

class Obj : public QLabel
{
    Q_OBJECT
public:
    explicit Obj(int k,int id,int r);
    QPoint pos;//此时的位置
    int r;//距离原点的距离
    int rank,speed;

    bool operator<(const Obj& a)
    {
        return r<a.r;
    }

signals:

};

#endif // OBJ_H
