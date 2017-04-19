#-------------------------------------------------
#
# Project created by QtCreator 2015-01-03T22:51:47
#
#-------------------------------------------------

QT       -= core

TARGET = asynchronous

#CONFIG += c++11

TEMPLATE = app

INCLUDEPATH += "../../include"

SOURCES += main.cpp 

HEADERS += 

FORMS += 

unix:!macx {
    LIBS += /usr/lib/x86_64-linux-gnu/libasound.so
}
win32 {
    LIBS += winmm.lib
}


