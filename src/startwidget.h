#ifndef STARTWIDGET_H
#define STARTWIDGET_H

#include <QWidget>
#include <QMediaPlayer>
#include "enter_mainscene.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Startwidget; }
QT_END_NAMESPACE

class Startwidget : public QWidget
{
    Q_OBJECT

public:
    Startwidget(QWidget *parent = nullptr);
    ~Startwidget();
    void paintEvent(QPaintEvent*);
private:
    MainScene *mainscene;
    Mybtn* StartBtn;
    Mybtn* MuteBtn;
    QMediaPlayer* player;

private:
    Ui::Startwidget *ui;
};
#endif // STARTWIDGET_H
