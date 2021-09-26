#ifndef AI_H
#define AI_H

class AIsteps
{
public:
    AIsteps(int xx,int yy);
    AIsteps(int xx=0);
    int pos;
    int x;
    int y;
    bool operator<(const AIsteps& a)const;
    static int trans_prio(bool enemy,int len);
    static int hash(int x,int y);
};

//优先级转化函数
//优先级
//己方4子>对方4子>对方3子>己方3子>对方2子>己方2子>己方1子>对方1子
//   8     7      6      5      4      3      2      1
//取出优先级最高的点位
#endif // AI_H
