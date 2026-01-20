/////////怪物类/////////
#include "monster.h"
#include "QDebug"
Monster::Monster(Mario* mario_,Brick* brick_,Pipe* pipe_)
{
    mario=mario_;
    brick=brick_;
    pipe=pipe_;
    int m[15]={650,950,1200,2030,2600,3300,3800,4140,5200,5900,6040,6180,6300,7000,7440};//monster只在地面上有
        QVector<int>v;
        for(int i=0;i<15;++i)
        {
            v.push_back(m[i]+500);
            v.push_back(2);//2代表尚未被踩，1代表被踩了但还没播放被踩扁的动画，0代表已播放被踩扁的动画，消失
            v.push_back(1);//0代表朝左，1代表朝右
            v.push_back(0);//monster's dieState
            mp.push_back(v);
            v.clear();
        }
        //    qDebug()<<mario->direction;
        //    for(QVector<QVector<int> >::iterator itp=pipe->mp.begin();itp!=pipe->mp.end();itp++)
        //        qDebug()<<*(itp->begin());
}
void Monster::MonsterMove()
{
    for(QVector<QVector<int> >::iterator it=mp.begin();it!=mp.end();it++)
    {
        int *x=it->begin(),*state=it->begin()+1,*direction=it->begin()+2,*dieState=it->begin()+3;
        int monsterWindowX=*x-mario->x;
        //压扁效果的计时
        if(*dieState!=0)
            *dieState+=1;
        if(*state==1 && *dieState==20)
        {
            *state=0;
            *dieState=0;
        }
        if(*state==2)
        {
            //移动
            if(*direction==0)
                *x-=1;
            if(*direction==1)
                *x+=1;
            if(*x==50)
                *direction=1;//monster移动到地图最左侧时，改变方向
            //monster针对mario的对策

            //mario踩扁monster，并弹跳起
            if(mario->windowX+35>=monsterWindowX && mario->windowX<=monsterWindowX+35 //x坐标约束
                    && mario->y<=500-50-40 && mario->y>=500-50-40-40 && //y坐标约束
                    mario->jumpHeight<=0)//在下落过程中
            {
                *state=1;//monster变为被踩扁的状态
                *dieState=1;//死亡计时
                if(!Music::isOnMute)
                      musicControl.beatEnemy->play();
                mario->isJumpEnd=false;
                mario->jumpHeight=19;//起跳
                mario->score+=15;
                return;
            }
            //mario触碰到monster，失去一条命，触发无敌帧
            if(mario->y==500-50-45//mario站在地面上
                    && !mario->isInvicible//mario不处于无敌状态
                    && mario->windowX+35>=monsterWindowX
                    && mario->windowX<=monsterWindowX+35
                    )
            {
                mario->isDie=true;
                //qDebug()<<"called";
            }
            //monster针对brick的对策
            for(QVector<QVector<int> >::iterator itb=brick->mp.begin();itb!=brick->mp.end();itb++)
            {
                int brickX=*(itb->begin()+0);
                int brickY=*(itb->begin()+1);
                int brickType=*(itb->begin()+2);
                int brickState=*(itb->begin()+3);
                if(brickState==1||(brickType!=0&&brickState==0))
                {
                    if(brickY+40>=500-50-40)
                    {
                        if(*direction==0&&(*x-brickX==60))
                        {
                            *direction=1;
                            return;
                        }
                        if(*direction==1&&(brickX-*x==50))
                        {
                            *direction=0;
                            return;
                        }
                    }
                }
            }
            //monster针对pipe的对策
            for(QVector<QVector<int> >::iterator itp=pipe->mp.begin();itp!=pipe->mp.end();itp++)
            {
                int pipeX=*(itp->begin());
                if(*direction==0 && (*x-pipeX==pipe->widthLong))
                {
                    *direction=1;
                    return;
                }
                if(*direction==1 && (pipeX-*x==40))
                {
                    *direction=0;
                    *x-=1;
                    return;
                }

            }
        }
    }
}
