#-------------------------------------------------
#
# Project created by QtCreator 2022-06-14T23:32:13
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SuperMario
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        gamebeginwindow.cpp \
    mybutton.cpp \
    mainscene.cpp \
    aboutscene.cpp \
    mario.cpp \
    brick.cpp \
    pipe.cpp \
    castle.cpp \
    pausescene.cpp \
    monster.cpp \
    gameoverscene.cpp \
    music.cpp

HEADERS += \
        gamebeginwindow.h \
    mybutton.h \
    mainscene.h \
    aboutscene.h \
    mario.h \
    brick.h \
    pipe.h \
    castle.h \
    pausescene.h \
    monster.h \
    gameoverscene.h \
    music.h

FORMS += \
        gamebeginwindow.ui \
    mainscene.ui \
    aboutscene.ui
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
RESOURCES += \
    res.qrc \
    res1.qrc \
    res1.qrc
