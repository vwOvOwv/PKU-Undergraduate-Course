////////音乐控制类////////
#ifndef MUSIC_H
#define MUSIC_H
#include"mybutton.h"
#include <QWidget>
#include<QSound>
#include<QSoundEffect>


class Music : public QWidget
{
    Q_OBJECT
public:
    explicit Music(QWidget *parent = 0);
    QSoundEffect*beatEnemy;
    QSoundEffect*bloodRewarding;
    QSoundEffect*breakBrick;
    QSoundEffect*die;
    QSoundEffect*eatMushroom;
    QSoundEffect*fail;
    QSoundEffect*finish;
    QSoundEffect*gameBegin;
    QSoundEffect*getCoin;
    QSoundEffect*mainMusic;
    QSoundEffect*ButtonClicked;
    static bool isOnMute;//静音状态标记
signals:

public slots:

};

#endif // MUSIC_H
