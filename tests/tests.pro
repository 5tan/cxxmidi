#-------------------------------------------------
#
# Project created by QtCreator 2015-01-03T22:51:47
#
#-------------------------------------------------

QT += widgets testlib

CONFIG += c++11

INCLUDEPATH += "../include"

SOURCES += \  
    tests.cpp

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
    ../include/cxxmidi/guts/unused.hpp\
    ../include/cxxmidi/utils.hpp\
    ../include/cxxmidi/sleep.hpp\
    ../include/cxxmidi/guts/thread.hpp\
    ../include/cxxmidi/guts/mutex.hpp\
    ../include/cxxmidi/event.hpp \
    ../include/cxxmidi/instrument.hpp \
    ../include/cxxmidi/note.hpp \
    ../include/cxxmidi/guts/compiler.hpp \
    ../include/cxxmidi/sysex.hpp \
    ../include/cxxmidi/version.hpp

FORMS += 

unix:!macx {
    LIBS += /usr/lib/x86_64-linux-gnu/libasound.so
    QMAKE_CXXFLAGS += -pedantic -Wall
}
win32 {
    LIBS += winmm.lib
}
