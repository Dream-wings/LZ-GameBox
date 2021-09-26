#ifndef REGISTER_SUCCESS_SCENE_H
#define REGISTER_SUCCESS_SCENE_H
#include <QDialog>
#include <QPaintEvent>
#include <QPainter>
#include <QTextEdit>
#include <QtDebug>
#include <QLabel>
#include "mybtn.h"
class Register_success_scene : public QDialog
{

public:
    Register_success_scene();
    void  paintEvent(QPaintEvent*);
    QLabel *success_text;
};

#endif // REGISTER_SUCCESS_SCENE_H
