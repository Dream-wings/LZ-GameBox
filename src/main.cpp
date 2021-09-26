#include "startwidget.h"
#include "piperepairer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::addLibraryPath("./plugins");
    Startwidget w;
    w.show();
    return a.exec();
}
