#ifndef DATACONFIG_H
#define DATACONFIG_H

#include <QObject>

class dataconfig : public QObject
{
    Q_OBJECT
public:
    explicit dataconfig(QObject *parent = nullptr);
    struct Config{
        int k,id,r;
    }data[20][20];
    int sum[20];
    int time[20];
signals:

};

#endif // DATACONFIG_H
