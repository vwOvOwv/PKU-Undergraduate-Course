////////////游戏结束页////////////
#ifndef GAMEOVERSCENE_H
#define GAMEOVERSCENE_H

#include <QWidget>
#include<QEvent>
#include<QPainter>
#include<QDialog>
#include<QMessageBox>
#include<QString>
#include<QLabel>
#include"mybutton.h"
#include"music.h"
class GameOverScene : public QDialog
{
    Q_OBJECT
public:
//    QString Info;//用于记录gameOver的信息
    QLabel label;//显示文字用
    Music musicControl;
    GameOverScene();
    void paintEvent(QPaintEvent *event);
signals:
    void back();//发出返回的信号
    void restart();//发出重启的信号
public slots:
};

#endif // GAMEOVERSCENE_H
