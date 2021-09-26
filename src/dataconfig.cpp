#include "dataconfig.h"

/*
 *  关卡配置
 *  0、大金块（1） 1、小金块（3） 2、迷你金块（7） 3、小石头（3） 4、大石头（2） 5、神秘袋子（4） 6、钻石（6）
 *  设置选择的斜率以及距远点的距离    r 一定要设置为对应速度的倍数！！！！
 *  每关矿物不超过20个
 *  id代表矿物种类  k代表斜率  r代表距离
 */


dataconfig::dataconfig(QObject *parent) : QObject(parent)
{
    //第一关,全是大金块，小金块，迷你金块
    sum[0]=9;
    time[0]=45;

    data[0][0].id=1,data[0][0].k=4,data[0][0].r=72;
    data[0][1].id=0,data[0][1].k=8,data[0][1].r=180;
    data[0][2].id=0,data[0][2].k=2,data[0][2].r=170;
    data[0][3].id=1,data[0][3].k=3,data[0][3].r=30;
    data[0][4].id=1,data[0][4].k=7,data[0][4].r=90;
    data[0][5].id=2,data[0][5].k=6,data[0][5].r=70;
    data[0][6].id=0,data[0][6].k=9,data[0][6].r=150;
    data[0][7].id=1,data[0][7].k=10,data[0][7].r=96;
    data[0][8].id=1,data[0][8].k=1,data[0][8].r=150;

    //第二关，加入了大石头，小石头
    sum[1]=12;
    time[1]=45;

    data[1][0].id=1,data[1][0].k=4,data[1][0].r=72;
    data[1][1].id=0,data[1][1].k=8,data[1][1].r=180;
    data[1][2].id=4,data[1][2].k=3,data[1][2].r=100;
    data[1][3].id=1,data[1][3].k=3,data[1][3].r=30;
    data[1][4].id=3,data[1][4].k=7,data[1][4].r=90;
    data[1][5].id=2,data[1][5].k=6,data[1][5].r=70;
    data[1][6].id=0,data[1][6].k=2,data[1][6].r=170;
    data[1][7].id=1,data[1][7].k=10,data[1][7].r=96;
    data[1][8].id=2,data[1][8].k=5,data[1][8].r=140;
    data[1][9].id=3,data[1][9].k=5,data[1][9].r=210;
    data[1][10].id=1,data[1][10].k=1,data[1][10].r=126;
    data[1][11].id=3,data[1][11].k=2,data[1][11].r=60;

    //第三关，加入了钻石，神秘袋子。
    sum[2]=14;
    time[2]=65;
    data[2][0].id=1,data[2][0].k=4,data[2][0].r=72;
    data[2][1].id=0,data[2][1].k=8,data[2][1].r=80;
    data[2][2].id=4,data[2][2].k=3,data[2][2].r=100;
    data[2][3].id=1,data[2][3].k=3,data[2][3].r=30;
    data[2][4].id=3,data[2][4].k=7,data[2][4].r=90;
    data[2][5].id=2,data[2][5].k=6,data[2][5].r=70;
    data[2][6].id=0,data[2][6].k=1,data[2][6].r=180;
    data[2][7].id=1,data[2][7].k=10,data[2][7].r=96;
    data[2][8].id=2,data[2][8].k=5,data[2][8].r=140;
    data[2][9].id=3,data[2][9].k=5,data[2][9].r=210;
    data[2][10].id=1,data[2][10].k=1,data[2][10].r=120;
    data[2][11].id=6,data[2][11].k=2,data[2][11].r=126;
    data[2][12].id=6,data[2][12].k=8,data[2][12].r=60;
    data[2][13].id=5,data[2][13].k=3,data[2][13].r=60;


}
