#-------------------------------------------------
#
# Project created by QtCreator 2015-01-03T22:51:47
#
#-------------------------------------------------

QT       += gui widgets

TARGET = qtmidieditor

TEMPLATE = app

INCLUDEPATH += "../../include"

SOURCES += main.cpp \ 
    filemodel.cpp \
    fileview.cpp \
    trackmodel.cpp \
    trackview.cpp \
    mainwindow.cpp

HEADERS += \ 
    filemodel.h \
    fileview.h \
    trackmodel.h \
    trackview.h \
    mainwindow.h

FORMS += \ 
    mainwindow.ui

unix:!macx {
    LIBS += /usr/lib/x86_64-linux-gnu/libasound.so
}
win32 {
    LIBS += winmm.lib
}


