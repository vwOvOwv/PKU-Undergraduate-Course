////////////砖块类(包含问号砖和普通砖)/////////
#ifndef BRICK_H
#define BRICK_H
#include<QVector>
#include<QDebug>
class Brick
{
public:
    Brick();
    QVector< QVector<int> > mp;
    int unknownState;
    void BrickStateChange();
};

#endif // BRICK_H
