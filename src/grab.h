#ifndef GRAB_H
#define GRAB_H

#include <QLabel>
#include <QTimer>

struct Revolve
{
    int dx,dy; //钩子的斜率
    QString Grabpath;//对应斜率所代表钩子图片
    QPoint pos;
};

class Grab : public QLabel
{
    Q_OBJECT
private:

public:
    void init(int i);
    explicit Grab(QWidget *parent = nullptr);

    static Revolve standard[11];

signals:

};

#endif // GRAB_H
