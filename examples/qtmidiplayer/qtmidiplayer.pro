#-------------------------------------------------
#
# Project created by QtCreator 2015-01-03T22:51:47
#
#-------------------------------------------------

QT       += gui widgets

TARGET = qtmidiplayer

TEMPLATE = app

INCLUDEPATH += "../../include"

SOURCES += main.cpp \ 
    mainwindow.cpp \
    playerheartbeatcallback.cpp \
    playerfinishedcallback.cpp

HEADERS += \
    mainwindow.h \
    playerheartbeatcallback.h \
    playerfinishedcallback.h

FORMS += \ 
    mainwindow.ui

unix:!macx {
    LIBS += /usr/lib/x86_64-linux-gnu/libasound.so
}
win32 {
    LIBS += winmm.lib
}


