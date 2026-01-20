//////////封装出一个新的按钮类//////////
#ifndef MYBUTTON_H
#define MYBUTTON_H

#include<QWidget>
#include<QString>
#include<QPushButton>
#include<QObject>
#include<QPropertyAnimation>
#include<QEvent>
#include<QTimer>
#include<QDebug>
class MyButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MyButton(QPushButton *parent = nullptr);
    //重载构造函数，将图片路径传入按钮内部
    MyButton(QString InitImg,QString EndImg="");
    void ZoomUp();
    void ZoomDown();//按钮弹起、落下
private:
    QString InitImgPath;//初始图片路径
    QString EndImgPath;//按下后显示图片路径，构造时默认为空

signals:

public slots:
};

#endif // MYBUTTON_H
