/////////水管类//////////
#ifndef PIPE_H
#define PIPE_H

#include<QVector>
class Pipe
{
public:
    Pipe();
    QVector< QVector<int> >mp;
    int heightLong;
    int heightshort;
    int widthLong;
    int widthShort;
};

#endif // PIPE_H
