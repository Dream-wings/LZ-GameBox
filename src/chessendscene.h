#ifndef CHESSENDSCENE_H
#define CHESSENDSCENE_H
#include <QPaintEvent>
#include <QDialog>
#include <QLabel>
#include "mybtn.h"
class ChessEndScene : public QDialog
{
private:
    QLabel *lab;
public:
    ChessEndScene();
    void paintEvent(QPaintEvent* p);
    Mybtn *restartbtn;
    Mybtn *continuebtn;
    void show(bool col);
};

#endif // CHESSENDSCENE_H
