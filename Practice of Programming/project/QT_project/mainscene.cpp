////////////游戏主界面////////////
#include "mainscene.h"
#include "ui_mainscene.h"
MainScene::MainScene(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainScene)
{
    ui->setupUi(this);
    this->setFixedSize(1000,500);
    this->setWindowTitle("SuperMario");

    SetGameOverScene();
    SetWholeGame();
    SetPauseScene();
    SetButtons();
    //timerNormal=startTimer(25);
    //timerFast=startTimer(15);当且仅当加速时开启
}
void MainScene::SetPauseScene()
{
    //实现继续按钮
    MyButton*continueBtn=new MyButton(":/Image/continueBtn.png");
    continueBtn->setParent(&pauseScene);
    continueBtn->move(0.5*pauseScene.width()-0.5*continueBtn->width(),0.1*pauseScene.height());
    connect(continueBtn,&MyButton::clicked,[=](){
        if(!Music::isOnMute)
            musicControl.ButtonClicked->play();
        continueBtn->ZoomUp();
        continueBtn->ZoomDown();
//        musicControl.mainMusic->setVolume(musicControl.isOnMute?0.0f:1.0f);
        //先实现按钮弹跳特效，而后等待0.5s后发送back()信号
        QTimer::singleShot(500,this,[=](){
            //继续播放音乐
            musicControl.mainMusic->setVolume(Music::isOnMute?0.0f:1.0f);
            emit pauseScene.back();
        });
    });
    //实现退出按钮
    MyButton *exitBtn=new MyButton(":/Image/exitBtn.png");
    exitBtn->setParent(&pauseScene);
    exitBtn->move(0.5*pauseScene.width()-0.5*exitBtn->width(),0.5*pauseScene.height());
    connect(exitBtn,&MyButton::clicked,
            [=](){
        if(!Music::isOnMute)
              musicControl.ButtonClicked->play();
        exitBtn->ZoomUp();
        exitBtn->ZoomDown();
        //先暂停0.5s，弹出一个是否选择退出的问题对话框
        QTimer::singleShot(500,this,[=](){
            int ret=QMessageBox::question(this,"Quit",
                                          "Do you really want to quit the game?",
                                          QMessageBox::No,
                                          QMessageBox::Yes);
            switch (ret) {
            case QMessageBox::Yes:
                this->close();//如果选Yes，则退出游戏；否则回到游戏
                break;
            default:
//                musicControl.mainMusic->setLoops(10000);
//                musicControl.mainMusic->play();
                break;
            }
        });
    });
}
void MainScene::SetButtons()
{
    //实现返回按钮
    MyButton*backBtn=new MyButton(":/Image/backBtn.png");
    backBtn->setParent(this);
    backBtn->move(0.945*this->width(),0.89*this->height());
    connect(backBtn,&MyButton::clicked,[=](){
        if(!Music::isOnMute)
            musicControl.ButtonClicked->play();
        backBtn->ZoomUp();
        backBtn->ZoomDown();
        //先实现按钮弹跳特效，而后等待0.5s后发送back()信号
        QTimer::singleShot(500,this,[=](){
            musicControl.mainMusic->stop();
            emit back();
            //musicControl.gameBegin->setLoops(10000);
            if(!Music::isOnMute)
                musicControl.gameBegin->play();
            if(!timerFastKilled)
                killTimer(timerFast);
            killTimer(timerNormal);
        });
    });
    //实现暂停按钮
    MyButton*pauseBtn=new MyButton(":/Image/pauseBtn.png");
    pauseBtn->setParent(this);
    pauseBtn->move(0.945*this->width()-50,0.89*this->height());
    connect(pauseBtn,&MyButton::clicked,[=](){
        if(!Music::isOnMute)
            musicControl.ButtonClicked->play();
//         qDebug()<<"called";
        pauseBtn->ZoomUp();
        pauseBtn->ZoomDown();
        musicControl.mainMusic->setVolume(0.0f);
        QTimer::singleShot(500,this,[=](){
//            qDebug()<<musicControl.mainMusic->volume();
            killTimer(timerNormal);
            if(!timerFastKilled)
                killTimer(timerFast);
            timerFastKilled=true;
            pauseScene.setParent(this);
            pauseScene.move(0.5*this->width()-0.5*pauseScene.width(),0.5*this->height()-0.5*pauseScene.height());
            pauseScene.open();
            //关闭计时器，减小音乐音量，实现暂停
        });
    });
    //继续
    connect(&pauseScene,&PauseScene::back,[=](){
        pauseScene.close();
        timerNormal=startTimer(25);
        //重新开启计时器
    });
    //实现音乐设置按钮
    MyButton*musicBtn=new MyButton(":/Image/musicBtn.png");
    musicBtn->setParent(this);
    musicBtn->move(0.945*this->width()-100,0.89*this->height());
    connect(musicBtn,&MyButton::clicked,[=](){
        if(!Music::isOnMute)
              musicControl.ButtonClicked->play();
        musicBtn->ZoomUp();
        musicBtn->ZoomDown();
        if(Music::isOnMute==false){
            musicControl.mainMusic->setVolume(0.0f);
            Music::isOnMute=true;
        }
        else{
            musicControl.mainMusic->setVolume(1.0f);
            Music::isOnMute=false;
        }
//        QTimer::singleShot(500,this,[=](){
//        });
    });
}

void MainScene::SetWholeGame()
{
    brick=new Brick;
    castle=new Castle;
    mario=new Mario;
    pipe=new Pipe;
    monster=new Monster(mario,brick,pipe);//游戏画面各组成要素
    gameProgress=true;//判断游戏是否正在进行
    timerFastKilled=true;//判断加速计时器是否开启
    time=0;
    isWin=false;
    key="NULL";
    mario->score=0;
    coinNum=0;
}
//重写paintEvent事件
void MainScene::paintEvent(QPaintEvent *event)
{
    //qDebug()<<event;
    //创建画家，指定绘图设备,载入图像
    QPainter painter(this);
    QPixmap pixBackground(":/Image/background.png");//背景
    QPixmap pixGround(":/Image/ground.png");//地面
    QPixmap pixMarioLeft(":/Image/moveleft.png");//mario向左走
    QPixmap pixMarioRight(":/Image/moveright.png");//mario向右走
    QPixmap pixMarioDie(":/Image/mariodie.png");//mario的死亡动画
    QPixmap pixUnknownBrick(":/Image/unknownbrick.png");//问号砖
    QPixmap pixUnknownBrickAfter(":/Image/unknownbrickafter.png");//被mario顶破后的问号砖
    QPixmap pixNormalBrick(":/Image/normalbrick.png");//普通砖块
    QPixmap pixMonsterLeft(":/Image/monsterleft.png");//怪物
    QPixmap pixMonsterRight(":/Image/monsterright.png");
    QPixmap pixLongPipe(":/Image/longpipe.png");//长水管
    QPixmap pixShortPipe(":/Image/shortpipe.png");//短水管
    QPixmap pixCasle(":/Image/castle.png");//城堡/终点
    QPixmap pixCoin(":/Image/coin.png");//金币
    //设置字体
    painter.setFont(QFont("Times",25,QFont::Black));
    //绘制背景图
    painter.drawPixmap(0,0,this->width(),this->height(),pixBackground);
    //绘制时间
    painter.drawText(10,30,"TIME:"+QString::number(time,'f',1));
    //绘制生命值
    painter.drawText(10,60,"LIFE:"+QString::number(mario->life,'f',0));
    //绘制得分
    painter.drawText(10,90,"SCORE:"+QString::number(mario->score,'f',0));
    //绘制得金币数
    painter.drawText(10,120,"COIN:"+QString::number(coinNum,'f',0));
    //绘制地面
    painter.drawPixmap(-5,450,pixGround,mario->goundState,0,1005,50);//截取自goundX始长1000pix的图片，以营造出动画效果
    //绘制砖块
    for (QVector < QVector < int >> ::iterator it = brick->mp.begin(); it != brick->mp.end();it++)
    {
        if (*(it->begin()) - mario->x > -50 && *(it->begin()) - mario->x < 1000)
        {
            if(*(it->begin()+2)==0 && *(it->begin()+3)==1)//如果该砖块是普通砖块且尚未被摧毁
                painter.drawPixmap(*(it->begin()) - mario->x, *(it->begin() + 1), pixNormalBrick);
            if(*(it->begin()+2)!=0 && *(it->begin()+3)==1)//如果该砖块是未知砖块且尚未被破坏
                painter.drawPixmap(*(it->begin()) - mario->x, *(it->begin() + 1),pixUnknownBrick,brick->unknownState,0,50,40);
            if(*(it->begin()+2)!=0 && *(it->begin()+3)==0)//如果该砖块是未知砖块且已被破坏
                painter.drawPixmap(*(it->begin()) - mario->x, *(it->begin() + 1), pixUnknownBrickAfter);
        }
    }
    //绘制金币
    for (QVector < QVector < int >> ::iterator it = brick->mp.begin(); it != brick->mp.end();it++)
    {
        if (*(it->begin()) - mario->x > -50 && *(it->begin()) - mario->x < 1000)
        {
            if(*(it->begin()+2)==1 && *(it->begin()+4)!=0)//如果该砖块带金币且已被顶起
                painter.drawPixmap(*(it->begin())-mario->x+10,//该砖块的x坐标
                                   *(it->begin() +1)-pixCoin.height()-*(it->begin()+4)*10,//该砖块的y坐标再减去升起的距离
                                   pixCoin,
                                   *(it->begin()+4)*30,0,30,33
                                   );
        }
    }
    //绘制水管
    for (QVector < QVector < int >> ::iterator it = pipe->mp.begin(); it != pipe->mp.end();it++)
    {
        if (*(it->begin()) - mario->x > -100 && *(it->begin()) - mario->x < 1000)
        {
            if(*(it->begin()+1)==0)
                painter.drawPixmap(*(it->begin())-mario->x,500-pixGround.height()-pipe->heightshort,pixShortPipe);
            else
                painter.drawPixmap(*(it->begin())-mario->x,500-pixGround.height()-pipe->heightLong,pixLongPipe);
        }
    }

    //绘制casle/终点
    if(castle->x-mario->x>-300 && castle->x-mario->x<1000)
        painter.drawPixmap(castle->x-mario->x,castle->y,pixCasle.width(),pixCasle.height(),pixCasle);
    //绘制monster
    for(QVector < QVector < int >> ::iterator it=monster->mp.begin();it!=monster->mp.end();it++)
    {
        int monsterX=*(it->begin()),
                monsterState=*(it->begin()+1),
                monsterDirection=*(it->begin()+2);
                //monsterDieState=*(it->begin()+3);
        if(monsterX-mario->x>-100 && monsterX-mario->x<1000)
        {
            if(monsterState==2)
            {
                if(monsterDirection==0)
                    painter.drawPixmap(monsterX-mario->x,500-pixGround.height()-pixMonsterLeft.height(),pixMonsterLeft);
                else
                    painter.drawPixmap(monsterX-mario->x,500-pixGround.height()-pixMonsterRight.height(),pixMonsterRight);
            }
            else if(monsterState==1)
            {
                if(monsterDirection==0)
                    painter.drawPixmap(monsterX-mario->x,500-pixGround.height()-pixMonsterLeft.height()*0.5,pixMonsterLeft.width(),pixMonsterLeft.height()*0.5,pixMonsterLeft);
                else
                    painter.drawPixmap(monsterX-mario->x,500-pixGround.height()-pixMonsterRight.height()*0.5,pixMonsterRight.width(),pixMonsterRight.height()*0.5,pixMonsterRight);
            }
        }
    }
    //绘制mario
    if(!mario->isDie && mario->invincibleState%2==0 && !isWin)//无敌帧时mario会闪烁，普通情况下invi*恒为
    {
        if(mario->direction=="left")
            painter.drawPixmap(mario->windowX,mario->y,pixMarioLeft,mario->walkState,0,45,45);//朝向为左
        else if(mario->direction=="right")
            painter.drawPixmap(mario->windowX,mario->y,pixMarioRight,mario->walkState,0,45,45);//朝向为右
    }
    else if(mario->isDie)
    {
        painter.drawPixmap(mario->windowX,mario->y,pixMarioDie,mario->dieState,0,45,45);
    }
}
//重写计时事件(动画效果主要通过计时事件和绘图事件的结合来实现)
void MainScene::timerEvent(QTimerEvent *event)
{
    int timerid=event->timerId();
    if(timerid==timerNormal&&mario->isDie)//如果mario死亡，则实现其死亡画面
    {
        mario->MarioDie();
        monster->MonsterMove();
        brick->BrickStateChange();
        if(!musicControl.isOnMute){
            musicControl.mainMusic->setVolume(0.0f);
            musicControl.die->play();
            musicControl.mainMusic->setVolume(1.0f);
        }
        key="NULL";
        time+=0.025;
        update();
        return;
    }
    if(timerid==timerNormal)
    {
        GameOver();
        mario->InvicibleSet();
        mario->MarioMove(key);
        mario->MarioJump();
        CollisionCheckJumpDown();
        CollisionCheckJumpUp();
        CollisionCheckMove();
        monster->MonsterMove();
        brick->BrickStateChange();
        time+=0.025;
        update();
    }
    if(timerid==timerFast)
    {
        mario->MarioMove(key);
        mario->MarioJump();
        CollisionCheckJumpDown();
        CollisionCheckJumpUp();
        CollisionCheckMove();
    }


}
//重写键盘事件
void MainScene::keyPressEvent(QKeyEvent *event)
{
    if(!mario->isDie)//只有在mario没死时，才会检测键盘输入
    {
        switch (event->key()) {
        case Qt::Key_Right:
            mario->direction=key="right";
            break;
        case Qt::Key_Left:
            mario->direction=key="left";
            break;
        case Qt::Key_Space:
            mario->isJump=true;
            //mario->isSpaceRelease=false;
            break;
        case Qt::Key_K:
            timerFast=startTimer(15);//开始加速
            timerFastKilled=false;
            break;
        default:
            break;
        }
    }
}
void MainScene::keyReleaseEvent(QKeyEvent *event)
{
    if(!mario->isDie)//只有在mario没死时，才会检测键盘输入
    {
        switch (event->key()) {
        case Qt::Key_Right:
            key="NULL";//不必把direction也清空，因为direction是个持续的状态
            mario->walkState=0;//将mario复原为站立的状态
            break;
        case Qt::Key_Left:
            key="NULL";
            mario->walkState=0;
            break;
        case Qt::Key_Space:
            mario->isJump=false;//此时isJumpEnd仍未复原，是以仍能防止二段跳
            mario->isSpaceRelease=true;
            break;
        case Qt::Key_K:
            killTimer(timerFast);
            timerFastKilled=true;//关闭加速计时器，也就是说加速只在持续按下K时生效
            break;
        default:
            break;
        }
    }
}
//上升时碰撞检测
void MainScene::CollisionCheckJumpUp()
{
    //垂直方向
    if(mario->jumpHeight>0)//如果此时mario仍在上升，需要判定其是否会顶到砖块
    {
        //砖块
        int brickY,brickX,brickType,brickState,brickWindowX,dY;
        for (QVector < QVector < int >> ::iterator it = brick->mp.begin(); it != brick->mp.end();it++)
        {
            brickX=*(it->begin()+0);
            brickY=*(it->begin()+1)+40;//+40,为砖块底部的Y坐标
            brickType=*(it->begin()+2);
            brickState=*(it->begin()+3);
            brickWindowX=brickX-mario->x;//brick相对于屏幕的坐标
            dY=-mario->y+brickY;//800 390 450
            if(mario->windowX+40>=brickWindowX&&mario->windowX<=brickWindowX+35&&dY>=-11&&dY<=11)//dY有+-11的余地是因为mario跳跃时Y最多一次变化20
            {
                if(brickState==1)
                {
                    mario->score+=5;
                    if(brickType==1)//顶破有金币的砖块
                    {
                        if(!Music::isOnMute)
                            musicControl.breakBrick->play();
                        mario->score+=5;
                        *(it->begin()+4)=1;//使金币被顶起
                    }
                    mario->jumpHeight=0;//开始下落
                    mario->y=brickY;
                    *(it->begin()+3)=0;
                    if(*(it->begin()+2)==1){
                        coinNum++;
                        if(!Music::isOnMute)
                            musicControl.getCoin->play();
                    }
                    return;
                }
                if(brickState==0&&brickType!=0)
                {
                    mario->jumpHeight=0;//开始下落
                    mario->y=brickY;
                    return;
                }
            }
        }
    }

}
//下落时碰撞检测
void MainScene::CollisionCheckJumpDown()
{
    if(mario->jumpHeight<0)//mario下落，寻找落脚点
    {
        //地面
        if(mario->y>=405)
        {
            mario->isJumpEnd=true;
            mario->jumpHeight=20;
            mario->y=405;
            return;
        }
        //砖块
        int brickY,brickX,brickType,brickState,brickWindowX,dY;
        for (QVector < QVector < int >> ::iterator it = brick->mp.begin(); it != brick->mp.end();it++)
        {
            brickX=*(it->begin()+0);
            brickY=*(it->begin()+1)-45;//-45，即减去mario的身高
            brickType=*(it->begin()+2);
            brickState=*(it->begin()+3);
            brickWindowX=brickX-mario->x;
            dY=-mario->y+brickY;//800 390 450
            if(mario->windowX+40>=brickWindowX && mario->windowX<=brickWindowX+35 && dY>=-11 && dY<=11)//dY有+-11的余地是因为mario跳跃时Y最多一次变化20
            {
                //40是mario的宽度
                if(brickState==1||(brickState==0&&brickType!=0))
                {
                    mario->isJumpEnd=true;
                    mario->jumpHeight=0;
                    mario->y=brickY;
                    return;
                }
            }
        }
        //水管
        int pipeWindowX,tableHeight;
        for (QVector < QVector < int >> ::iterator it = pipe->mp.begin(); it != pipe->mp.end();it++)
        {
            if (*(it->begin()) - mario->x > -100 && *(it->begin()) - mario->x < 1000)
            {
                pipeWindowX=*(it->begin())-mario->x;
                if(*(it->begin()+1)==0)
                {
                    tableHeight=500-50-pipe->heightshort;//50为地面高度
                    if(mario->windowX+40>=pipeWindowX+5 && mario->windowX<=pipeWindowX+pipe->widthShort-15
                            && mario->y+45>=tableHeight-11 && mario->y+45<=tableHeight+11)//45为mario身高
                    {
                        mario->isJumpEnd=true;
                        mario->jumpHeight=0;
                        mario->y=tableHeight-45;
                        return;
                    }
                }
                else
                {
                    tableHeight=500-50-pipe->heightLong;//50为地面高度
                    if(mario->windowX+40>=pipeWindowX+5 && mario->windowX<=pipeWindowX+pipe->widthLong-15
                            && mario->y+45>=tableHeight-11 && mario->y+45<=tableHeight+11)//45为mario身高
                    {
                        mario->isJumpEnd=true;
                        mario->jumpHeight=0;
                        mario->y=tableHeight-45;
                        return;
                    }
                }
            }
        }
    }
}
//水平移动碰撞检测
void MainScene::CollisionCheckMove()
{
    //砖块
    int brickYup,brickYdown,brickX,brickType,brickState,brickWindowX;
    for (QVector < QVector < int >> ::iterator it = brick->mp.begin(); it != brick->mp.end();it++)
    {
        brickX=*(it->begin()+0);
        brickYup=*(it->begin()+1)-45;//-45，即减去mario的身高
        brickYdown=*(it->begin()+1)+40;
        brickType=*(it->begin()+2);
        brickState=*(it->begin()+3);
        brickWindowX=brickX-mario->x;
        //mario朝右时
        if(brickState==1||(brickState==0&&brickType!=0))
        {
            if(mario->y>=brickYup+10&&mario->y<=brickYdown-5)
            {
                if(mario->direction=="right")
                {
                    if(mario->windowX+40<=brickWindowX-2&&mario->windowX+40>=brickWindowX-5-2)
                    {
                        mario->canMove=false;
                        return;
                    }
                }
                if(mario->direction=="left")
                {
                    if(mario->windowX>=brickWindowX+35+2&&mario->windowX<=brickWindowX+40+2)
                    {
                        mario->canMove=false;
                        return;
                    }
                }
            }
        }

    }
    //水管
    int pipeWindowX,tableHeight;
    for (QVector < QVector < int >> ::iterator it = pipe->mp.begin(); it != pipe->mp.end();it++)
    {
        if (*(it->begin()) - mario->x > -100 && *(it->begin()) - mario->x < 1000)
        {
            pipeWindowX=*(it->begin())-mario->x;
            if(*(it->begin()+1)==0)
                tableHeight=500-50-pipe->heightshort;//50为地面高度
            else
                tableHeight=500-50-pipe->heightLong;
            if(mario->y<=450 && mario->y+50>=tableHeight-2)
            {
                if(mario->direction=="right")
                {
                    if(mario->windowX+40<=pipeWindowX+5-2&& mario->windowX+40>=pipeWindowX+5-2-5)
                    {
                        mario->canMove=false;
                        return;
                    }
                }
                if(mario->direction=="left")
                {
                    //widthshort=widthlong
                    if(mario->windowX>=pipeWindowX+pipe->widthShort-15+2 && mario->windowX<=pipeWindowX+pipe->widthShort-15+2+5)
                    {
                        mario->canMove=false;
                        return;
                    }
                }
            }

        }
    }
    mario->canMove=true;//复原，防止卡住
}

//处理游戏结束的弹窗
void MainScene::GameOver()
{
//    musicControl.mainMusic->stop();
    if(abs((castle->x-mario->x+100)-mario->windowX-20)<=50 && abs(castle->y+195-mario->y-45)<=50)
        isWin=true;
    if(mario->isGameOver||isWin)
    {
        if(!timerFastKilled)
            killTimer(timerFast);
        killTimer(timerNormal);
        QString text;
        if(isWin){
            text="\tYou Win!\nSpend "+QString::number(time,'f',1)+" seconds \nScore "+QString::number(mario->score,'f',0)+" points!";
            if(!Music::isOnMute)
                musicControl.finish->play();
        }
        else{
            text="You Lost!\nSorry!";
            if(!Music::isOnMute)
                musicControl.fail->play();//可能会与死亡音效交叠
        }
        gameoverScene.label.setText(text);//设置文字
        gameoverScene.label.adjustSize();//设置自动尺寸
        gameoverScene.label.move((gameoverScene.width()-gameoverScene.label.width())/2,gameoverScene.height()*0.2);//设置label居中位置
        QTimer::singleShot(500,this,[=](){
            gameoverScene.setParent(this);
            gameoverScene.move(0.5*this->width()-0.5*gameoverScene.width(),0.5*this->height()-gameoverScene.height()*0.5);
            gameoverScene.open();
            musicControl.mainMusic->stop();
            //qDebug()<<gameoverScene.Info;
        });

    }
}
void MainScene::SetGameOverScene()
{

    //实现退出按钮
    MyButton *exitGGBtn=new MyButton(":/Image/exitBtn.png");
    exitGGBtn->setParent(&gameoverScene);
    exitGGBtn->move(0.5*gameoverScene.width()-0.5*exitGGBtn->width(),0.7*gameoverScene.height());
    connect(exitGGBtn,&MyButton::clicked,
            [=](){
        if(!Music::isOnMute)
            musicControl.ButtonClicked->play();
        exitGGBtn->ZoomUp();
        exitGGBtn->ZoomDown();
        //先暂停0.5s，弹出一个是否选择退出的问题对话框
        QTimer::singleShot(500,this,[=](){
            int ret=QMessageBox::question(this,"Quit",
                                          "Do you really want to quit the game?",
                                          QMessageBox::No,
                                          QMessageBox::Yes);
            switch (ret) {
            case QMessageBox::Yes:
                this->close();//如果选Yes，则退出游戏；否则什么都不用做
                break;
            default:
                break;
            }
        });
    });
    //实现重启按钮
    MyButton*restartBtn=new MyButton(":/Image/continueBtn.png");
    restartBtn->setParent(&gameoverScene);
    restartBtn->move(0.5*gameoverScene.width()-0.5*restartBtn->width(),0.5*gameoverScene.height());
    connect(restartBtn,&MyButton::clicked,[=](){
        if(!Music::isOnMute)
            musicControl.ButtonClicked->play();
        restartBtn->ZoomUp();
        restartBtn->ZoomDown();
        //先实现按钮弹跳特效，而后等待0.5s后发送back()信号
        QTimer::singleShot(500,this,[=](){
            emit gameoverScene.restart();
        });
    });
    connect(&gameoverScene,&GameOverScene::restart,[=](){
        gameoverScene.close();
        timerNormal=startTimer(25);
        //重新开启计时器
        SetWholeGame();
        musicControl.mainMusic->stop();
        musicControl.mainMusic->setLoopCount(QSoundEffect::Infinite);
        musicControl.mainMusic->setVolume(Music::isOnMute?0.0f:1.0f);
        musicControl.mainMusic->play();
    });
}
MainScene::~MainScene()
{
//    killTimer(timerNormal);
//    if(!timerFastKilled)
//        killTimer(timerFast);
    delete ui;
    delete brick;
    delete castle;
    delete mario;
    delete pipe;
    delete monster;
}

