#ifndef MYBTN_H
#define MYBTN_H

#include <QPushButton>
#include <QMediaPlayer>

class Mybtn : public QPushButton
{
    Q_OBJECT
public:
    QString ImgPath[2];
    void load(QString a);
public:
    explicit Mybtn(QString a,QString b="",double ratio=1);//两个图片路径以及缩放比例
    QMediaPlayer* player;
    double ratio;
    void zoom();
    void mousePressEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);

signals:

};

#endif // MYBTN_H
