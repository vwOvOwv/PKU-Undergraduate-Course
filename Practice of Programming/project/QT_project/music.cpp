////////音效控制////////
#include "music.h"
//#include<gamebeginwindow.h>

Music::Music(QWidget *parent) : QWidget(parent)
{
    beatEnemy=new QSoundEffect;
    beatEnemy->setSource(QUrl::fromLocalFile(":/Music/beatenemy.wav"));
    breakBrick=new QSoundEffect;
    breakBrick->setSource(QUrl::fromLocalFile(":/Music/breakbrick.wav"));
    die=new QSoundEffect;
    die->setSource(QUrl::fromLocalFile(":/Music/die.wav"));
    fail=new QSoundEffect;
    fail->setSource(QUrl::fromLocalFile(":/Music/fail.wav"));
    finish=new QSoundEffect;
    finish->setSource(QUrl::fromLocalFile(":/Music/finish.wav"));
    gameBegin=new QSoundEffect;
    gameBegin->setSource(QUrl::fromLocalFile(":/Music/gamebegin.wav"));
    getCoin=new QSoundEffect;
    getCoin->setSource(QUrl::fromLocalFile(":/Music/getcoin.wav"));
    mainMusic=new QSoundEffect;
    mainMusic->setSource(QUrl::fromLocalFile(":/Music/mainmusic.wav"));
    ButtonClicked=new QSoundEffect;
    ButtonClicked->setSource(QUrl::fromLocalFile(":/Music/clicked.wav"));
}
bool Music::isOnMute=false;
