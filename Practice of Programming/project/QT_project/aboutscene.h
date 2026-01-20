///////////游戏的关于界面///////////
#ifndef ABOUTSCENE_H
#define ABOUTSCENE_H

#include <QWidget>
#include<QPainter>
#include"mybutton.h"
namespace Ui {
class AboutScene;
}

class AboutScene : public QWidget
{
    Q_OBJECT

public:
    explicit AboutScene(QWidget *parent = 0);
    ~AboutScene();
    //重写paintEvent事件，绘制背景图
    void paintEvent(QPaintEvent *event);
private:
    Ui::AboutScene *ui;
signals:
    void back();//返回开始页
};

#endif // ABOUTSCENE_H
