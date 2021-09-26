#ifndef PIPEREPAIRERENDSCENE_H
#define PIPEREPAIRERENDSCENE_H
#include <QWidget>
#include <QDialog>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include "mybtn.h"
class PipeRepairerEndScene : public QDialog
{
public:
    Mybtn* restartbtn;
    Mybtn* exitbtn;
    PipeRepairerEndScene();
    void paintEvent(QPaintEvent*);
};

#endif // PIPEREPAIRERENDSCENE_H
