//////////封装出一个新的按钮类//////////
#include "mybutton.h"
MyButton::MyButton(QPushButton *parent) : QPushButton(parent)
{

}
MyButton::MyButton(QString InitImg, QString EndImg)
{
    InitImgPath=InitImg;
    EndImgPath=EndImg;
    //创建Pixmap对象，载入图片，并检测是否能正常载入
    QPixmap pix;
    bool isLoad=pix.load(InitImgPath);
    if(!isLoad)
        qDebug()<<"无法载入按钮图片";
    //设置图片固定尺寸
    this->setFixedSize(pix.width(),pix.height());
    //设置图片样式表
    this->setStyleSheet("QPushButton{border:Opx;}");
    //设置图标
    this->setIcon(QIcon(pix));
    //设置图标大小
    this->setIconSize(QSize(pix.width(),pix.height()));

}
void MyButton::ZoomUp()//按钮弹起
{
    //创建动画对象
    QPropertyAnimation * animationUp = new QPropertyAnimation(this,"geometry");
    //设置时间间隔，单位毫秒
    animationUp->setDuration(200);
    //创建起始位置
    animationUp->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));
    //创建结束位置
    animationUp->setEndValue(QRect(this->x(),this->y()+5,this->width(),this->height()));
    //设置缓和曲线，QEasingCurve::OutBounce 为弹跳效果    animationUp->setEasingCurve(QEasingCurve::OutBounce);
    //开始执行动画
    animationUp->start();

}
void MyButton::ZoomDown()//落下
{
    QPropertyAnimation * animationDown =  new QPropertyAnimation(this,"geometry");
    animationDown->setDuration(200);
    animationDown->setStartValue(QRect(this->x(),this->y()+5,this->width(),this->height()));
    animationDown->setEndValue(QRect(this->x(),this->y(),this->width(),this->height()));
    animationDown->setEasingCurve(QEasingCurve::OutBounce);
    animationDown->start();
}

