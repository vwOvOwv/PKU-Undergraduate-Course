///////////游戏的关于界面///////////
#include "aboutscene.h"
#include "ui_aboutscene.h"
AboutScene::AboutScene(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutScene)
{
    ui->setupUi(this);
    this->setFixedSize(1000,500);
    this->setWindowTitle("About SuperMario");
    //实现返回按钮
    MyButton*backBtn=new MyButton(":/Image/backBtn.png");
    backBtn->setParent(this);
    backBtn->move(0.8*this->width(),0.76*this->height());
    connect(backBtn,&MyButton::clicked,[=](){
        backBtn->ZoomUp();
        backBtn->ZoomDown();
        //先实现按钮弹跳特效，而后等待0.5s后发送back()信号
        QTimer::singleShot(500,this,[=](){
            emit back();
        });
    });
}
//重写paintEvent事件，绘制背景图
void AboutScene::paintEvent(QPaintEvent *event)
{
    //创建画家，指定绘图设备
    QPainter painter(this);
    //创建背景图并绘制之
    QPixmap pix(":/Image/AboutScene.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
}
AboutScene::~AboutScene()
{
    delete ui;
}
