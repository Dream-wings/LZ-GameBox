#ifndef CHOICEOFDIFFICULTYSCENE_H
#define CHOICEOFDIFFICULTYSCENE_H
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <QDialog>
#include <QIcon>
#include "mybtn.h"

class ChoiceOfDifficultyScene : public QDialog
{
    Q_OBJECT
public:
    explicit ChoiceOfDifficultyScene(QWidget* parent=nullptr);
    void paintEvent(QPaintEvent *event);
    bool choice;
    Mybtn *easybtn;
    Mybtn *hardbtn;
};

#endif // CHOICEOFDIFFICULTYSCENE_H
