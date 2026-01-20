#include "gamebeginwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include<QSlider>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/Image/icon"));
    //增加打开游戏时的图片特效
    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/Image/openScene.png"));
    splash->show();
    gameBeginWindow w;
    QTimer::singleShot(500,splash,SLOT(close()));
    QTimer::singleShot(1000,&w,SLOT(show()));
    return a.exec();
}
