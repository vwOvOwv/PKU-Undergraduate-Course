//////////游戏结束页//////////
#include "gameoverscene.h"

GameOverScene::GameOverScene()
{
    //设置固定的窗口大小
    this->setFixedSize(300,400);
    //设置窗口标题
    this->setWindowTitle("GameOver");
    //设置窗口样式
    this->setWindowModality(Qt::ApplicationModal);
    //设置label字体
    label.setParent(this);
    QFont font("Times",30,QFont::Black);
    label.setFont(font);//设置字体
    label.setAlignment(Qt::AlignCenter);//设置居中
}
void GameOverScene::paintEvent(QPaintEvent *event)
{
    //创建画家，指定绘图设备
    QPainter painter(this);
    //创建背景图并绘制之
    QPixmap pix(":/Image/pauseScene.png");
//    //设置字体
//    painter.setFont(QFont("Times",10,QFont::Black));
//    //绘制文字
//    painter.drawText(0,0,"You Die");
    //画图
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
}
