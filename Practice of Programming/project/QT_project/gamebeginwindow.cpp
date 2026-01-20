////////游戏初始界面////////
#include "gamebeginwindow.h"
#include "ui_gamebeginwindow.h"
gameBeginWindow::gameBeginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::gameBeginWindow)
{
    ui->setupUi(this);
    //设置固定的窗口大小
    this->setFixedSize(1000,500);
    //设置应用图片
    this->setWindowIcon(QPixmap(":/Image/icon.ico"));
    //设置窗口标题
    this->setWindowTitle("SuperMario");
    //播放开始音乐
    musicControl.gameBegin->play();
    //设置开始按钮
    MyButton *startBtn=new MyButton(":/Image/startBtn.png");
    startBtn->setParent(this);
    startBtn->move(this->width()*0.8,this->height()*0.55);
    connect(startBtn,&MyButton::clicked,
            [=](){
        //qDebug()<<"1";
        startBtn->ZoomUp();
        startBtn->ZoomDown();
        //播放提示音
        musicControl.ButtonClicked->play();
        musicControl.gameBegin->stop();
        //实现弹起再落下的特效，暂停0.5s后，进入游戏主界面
        QTimer::singleShot(500,this,[=](){
            this->hide();
            mainScene.show();
            mainScene.musicControl.mainMusic->setLoopCount(10000);//无限循环
//            qDebug()<<mainScene.musicControl.isOnMute;
            mainScene.musicControl.mainMusic->setVolume(Music::isOnMute?0.0f:1.0f);
            mainScene.musicControl.mainMusic->play();
            mainScene.timerNormal=mainScene.startTimer(25);//开始计时
        });

    });
    //在主界面按下返回按钮后，询问是否确定返回，若是，返回初始界面
    connect(&mainScene,&MainScene::back,[=](){
//        int ret=QMessageBox::question(this,"Back",
//                                      "Do you really want to turn back to the initial scene?\n",
//                                      //"Your game progress cannot be saved",
//                                      QMessageBox::No,
//                                      QMessageBox::Yes);
//        switch (ret) {
//        case QMessageBox::Yes:
//            this->show();
//            mainScene.hide();
//            break;
//        default:
//            break;
//        }
        this->show();
        mainScene.hide();
        //继续播放开始界面音乐
//        musicControl.gameBegin->setLoops(10000);
        musicControl.gameBegin->play();
    });
    //设置退出按钮
    MyButton *exitBtn=new MyButton(":/Image/exitBtn.png");
    exitBtn->setParent(this);
    exitBtn->move(this->width()*0.81,this->height()*0.72);
    connect(exitBtn,&MyButton::clicked,
            [=](){
        //暂停开始界面音乐
        musicControl.gameBegin->stop();
        //播放提示音
        musicControl.ButtonClicked->play();
        //qDebug()<<"2";
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
                this->close();//如果选Yes，则退出游戏；否则返回开始界面，播放音乐
                break;
            default:
//                musicControl.gameBegin->play();
                break;
            }
        });
    });
    //设置关于按钮
    MyButton *aboutBtn=new MyButton(":/Image/aboutBtn.png");
    aboutBtn->setParent(this);
    aboutBtn->move(this->width()*0.86,this->height()*0.89);
    connect(aboutBtn,&MyButton::clicked,
            [=](){
        //qDebug()<<"2";
        aboutBtn->ZoomUp();
        aboutBtn->ZoomDown();
        //播放提示音
        musicControl.ButtonClicked->play();
        QTimer::singleShot(500,this,[=](){
            this->hide();
            aboutScene.show();
        });
    });
    //在关于界面按下返回按钮,返回初始页
    connect(&aboutScene,&AboutScene::back,[=]{
        //播放提示音
        musicControl.ButtonClicked->play();
        aboutScene.hide();
        this->show();
    });



}
//重写paintEvent事件，设置背景图片
void gameBeginWindow::paintEvent(QPaintEvent *ev)
{
    //创建画家，指定绘图设备
    QPainter painter(this);
    //创建背景图并绘制之
    QPixmap pix(":/Image/beginScene.jpg");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
}

gameBeginWindow::~gameBeginWindow()
{
    delete ui;
}
