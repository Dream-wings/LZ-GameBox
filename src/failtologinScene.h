#ifndef FAILTOLOGIN_H
#define FAILTOLOGIN_H
#include <QPaintEvent>
#include <QDialog>
class FailToLogin : public QDialog
{
public:
    FailToLogin();
    void  paintEvent(QPaintEvent*);
};
#endif // FAILTOLOGIN_H
