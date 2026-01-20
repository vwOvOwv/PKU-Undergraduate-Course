////////////游戏主界面////////////
#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QWidget>
#include<aboutscene.h>
#include<mybutton.h>
#include<QPainter>
#include<QDebug>
#include<QMessageBox>//标准对话框
#include <QWidget>
#include<QString>
#include<QTimer>
#include<QKeyEvent>
#include<QDialog>
#include<QLabel>
#include<QSound>//多媒体模块下的音效头文件
#include"pausescene.h"
#include"brick.h"
#include"castle.h"
#include"mario.h"
#include"pipe.h"
#include"monster.h"
#include"gameoverscene.h"
#include"music.h"
namespace Ui {
class MainScene;
}

class MainScene : public QWidget
{
    Q_OBJECT

public:
    PauseScene pauseScene;//暂停菜单
    GameOverScene gameoverScene;//游戏结束菜单
    Brick* brick;
    Castle* castle;
    Mario* mario;
    Pipe* pipe;
    Monster* monster;//游戏画面各组成要素
    Music musicControl;//游戏音乐音效
    int timerNormal;
    int timerFast;//两个计时器的编号，Fast用于加速，Normal用于常规绘图
    double time;//计算通关时间
    bool gameProgress;//判断游戏是否正在进行
    bool timerFastKilled;//判断加速计时器是否开启
    bool isWin;//用于判断游戏是否胜利
    int coinNum;//记录游戏所得金币数
    QString key;//用于记录键盘事件，当移动时为“left/right”，不移动时为“NULL”，以与mario->direction区分，后者记录的是mario的朝向
    explicit MainScene(QWidget *parent = 0);
    ~MainScene();
    //重写绘图事件
    void paintEvent(QPaintEvent *event);
    //重写计时事件(动画效果主要通过计时事件和绘图事件的结合来实现)
    void timerEvent(QTimerEvent *event);
    //重写键盘事件
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void SetButtons();//设置游戏主界面的pause,music,back按钮
    void SetPauseScene();//设置游戏的暂停画面
    void SetWholeGame();//设置游戏的主要参数(初始化)
    void CollisionCheckJumpUp();//碰撞检测
    void CollisionCheckJumpDown();
    void CollisionCheckMove();
    void GameOver();//用于处理跟游戏结束有关的事件
    void SetGameOverScene();//设置游戏结束的弹窗
private:
    Ui::MainScene *ui;
signals:
    void back();//返回上一页
};

#endif // MAINSCENE_H
