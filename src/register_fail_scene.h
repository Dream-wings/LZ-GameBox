#ifndef REGISTER_FAIL_SCENE_H
#define REGISTER_FAIL_SCENE_H
#include <QDialog>
#include <QPaintEvent>
#include <QPainter>
#include <QTextEdit>
#include <QtDebug>
#include <QLabel>
#include "mybtn.h"

class Register_fail_scene : public QDialog
{
public:
    Register_fail_scene();
    void  paintEvent(QPaintEvent*);
    QLabel *success_text;
};

#endif // REGISTER_FAIL_SCENE_H
