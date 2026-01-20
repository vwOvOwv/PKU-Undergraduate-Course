/////////怪物类/////////
#ifndef MOUNSTER_H
#define MOUNSTER_H
#include<QVector>
#include"mario.h"
#include"brick.h"
#include"pipe.h"
#include<music.h>
class Monster
{
public:
    Monster(Mario* mario_,Brick* brick_,Pipe* pipe_);//初始化，由于是用指针初始化的，所以不用在timeevent里更新地图的状态
    Mario* mario;
    Brick* brick;
    Pipe* pipe;
    QVector<QVector<int> >mp;
    Music musicControl;
    void MonsterMove();//移动
};

#endif // MOUNSTER_H
