////////马里奥类////////
#include "mario.h"

Mario::Mario()
{
    x=30;
    y=405;//mario的绝对坐标,由于是以左上角为坐标原点，故y=450-45，45为mario高度
    windowX=0;//mario与地面的相对坐标，实现地面的相对运动
    goundState=0;//地面的状态
    walkState=0;//mario运动的状态
    jumpHeight=20;//mario跳跃距离
    life=5;//mario的生命值
    isDie=false;//mario是否死亡
    direction="right";//mario运动的方向，左还是右
    isJump=false;//判断空格是否被按下
    isJumpEnd=true;//判断跳跃是否结束，以防止二段跳
    isSpaceRelease=true;//判断空格是否被松开，以检测滞空时间
    canMove=true;//能否移动，用于mario的碰撞检测
    dieState=0;//播放死亡动画时的状态
    invincibleState=0;//无敌帧的情况
    isInvicible=false;
    isGameOver=false;
    score=0;
}
void Mario::MarioJump()//弹跳
{
    if (isJump && isJumpEnd && isSpaceRelease) {
        isSpaceRelease = false;
        isJumpEnd = false;
        jumpHeight = 19;
        /*
         * 跳跃机制详解：当按下空格键后，isJump变为true，此时isJumpEnd仍为true，isSpaceRelease仍为true；
         * 是以此时触发跳跃机制，令isSpaceRelease = false;isJumpEnd = false;
         * 何解？isSpaceRelease变为false防止跳跃机制在按下一次空格键后再次触发，直至空格键被释放后复原；
         * isJumpEnd变为false,其一是为了在isSpaceRelease复原后仍控制跳跃机制不触发，其二是为了让mario在空中时保持体态不变
         * jumpHeight变为20直接触发跳跃机制，使y坐标变化，直到碰撞检测使将一切复原，停止跳跃；
         */
    }
    if (jumpHeight<20) {
        y -= jumpHeight;
        jumpHeight-=1;
    }
//    if(y>405)
//    {
//        isJumpEnd=true;
//        jumpHeight=20;
//        y=405;
//    }

}
void Mario::MarioMove(QString key)//水平移动,如果直接调用direction的话，会导致mario在没有移动时移动
{
    //qDebug()<<x;
    //刚开始时，mario有相对窗口的运动，当mario居中时，mario就相对窗口静止，相对地面运动
    if(key=="left"&&x<=400&&canMove&&x>=30)//如果x<30,mario可能会消失在边界
    {
        x-=5;
        windowX-=5;
        walkState+=57;
    }
    else if(key=="right"&&x<=400&&canMove)
    {
        x+=5;
        windowX+=5;
        walkState+=57;
    }
    else if((key=="left"&&x<30)||(key!="NULL"&&!canMove))//mario处于屏幕最左侧或者被卡着不动时，mario姿势变化，位置不变
    {
        walkState+=57;
    }
    else if(key=="right"&&x>400&&canMove)
    {
        x+=5;
        goundState+=5;
        walkState+=57;
    }
    else if(key=="left"&&x>400&&canMove)
    {
        x-=5;
        goundState-=5;
        walkState+=57;
    }
    //循环复位
    if(goundState>=1000)
        goundState=0;
    if(goundState<-5)
        goundState=995;
    if(walkState>=1140)
        walkState=0;
    //滞空时mario不做动作
    if(!isJumpEnd)
        walkState=0;
}

void Mario::MarioDie()//播放mario的死亡动画
{
    if(isDie)
    {
        dieState+=50;
        if(dieState>=1000)
        {
            dieState=0;
            isDie=false;
            life--;
            if(life==0)
                isGameOver=true;
            invincibleState=1;//无敌帧的初始化
            isInvicible=true;
        }
     }
}
void Mario::InvicibleSet()
{
    if(invincibleState==100)
    {
        invincibleState=0;
        isInvicible=false;
        return;
    }
    if(isInvicible)
        invincibleState++;
}
