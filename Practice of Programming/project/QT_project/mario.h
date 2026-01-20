////////马里奥类////////
#ifndef MARIO_H
#define MARIO_H
#include<QString>
#include<QDebug>
class Mario
{
public:
    Mario();
    int x,y;//mario的绝对坐标
    int windowX;//mario与窗口的相对坐标，实现地面的相对运动
    int life;//mario的生命值
    int score;//mario的得分
    int walkState;//mario的行走状态，决定选用行走图片的哪一部分
    int goundState;//地面的状态,主要目的是实现镜头跟随效果
    int dieState;//mario死亡动画的状态
    int jumpHeight;//mario跳跃的距离
    int invincibleState;//无敌帧的情况
    bool isInvicible;//是否无敌
    bool isGameOver;//判断游戏是否结束
    bool isDie;//mario是否死亡
    bool canMove;//mario是否可以移动(用于体积检测）
    bool isJump;//判断空格是否被按下
    bool isJumpEnd;//判断跳跃是否结束,以防止二段跳
    bool isSpaceRelease;//判断空格是否被松开，以检测滞空时间
    QString direction;//mario朝向，并不意味着运动，左还是右
    void MarioJump();//弹跳
    void MarioMove(QString key);//水平移动
    void MarioDie();//播放mario的死亡动画
    void InvicibleSet();//设置无敌帧
};

#endif // MARIO_H
