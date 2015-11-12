#-------------------------------------------------
#
# Project created by QtCreator 2015-01-03T22:51:47
#
#-------------------------------------------------

QT       += gui widgets

TARGET = test

TEMPLATE = app

INCLUDEPATH += "../include"

SOURCES += main.cpp 

HEADERS += \
    ../include/cxxmidi/time/duration.hpp\
    ../include/cxxmidi/time/period.hpp\
    ../include/cxxmidi/time/point.hpp\
    ../include/cxxmidi/file.hpp\
    ../include/cxxmidi/player/synchronous.hpp\
    ../include/cxxmidi/player/asynchronous.hpp\
    ../include/cxxmidi/player/abstract.hpp\
    ../include/cxxmidi/callback.hpp\
    ../include/cxxmidi/message.hpp\
    ../include/cxxmidi/track.hpp\
    ../include/cxxmidi/converters.hpp\
    ../include/cxxmidi/output/abstract.hpp\
    ../include/cxxmidi/output/default.hpp\
    ../include/cxxmidi/output/windows.hpp\
    ../include/cxxmidi/output/linux/alsa.hpp\
    ../include/cxxmidi/guts/endianness.hpp\
    ../include/cxxmidi/guts/simulator.hpp\
    ../include/cxxmidi/utils.hpp\
    ../include/cxxmidi/sleep.hpp\
    ../include/cxxmidi/guts/thread.hpp\
    ../include/cxxmidi/guts/mutex.hpp\
    ../include/cxxmidi/event.hpp \
    ../include/cxxmidi/instrument.hpp \
    ../include/cxxmidi/note.hpp

FORMS += 

unix:!macx {
    LIBS += /usr/lib/x86_64-linux-gnu/libasound.so
    DEFINES += CXXMIDI_LIL_ENDIAN
    QMAKE_CXXFLAGS += -pedantic -Wall
}
win32 {
    LIBS += winmm.lib
    DEFINES += CXXMIDI_LIL_ENDIAN
}


