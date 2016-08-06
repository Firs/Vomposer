#-------------------------------------------------
#
# Project created by QtCreator 2016-07-30T11:35:07
#
#-------------------------------------------------

QT       += multimedia

QT       -= gui

TARGET = Engine
TEMPLATE = lib

DEFINES += ENGINE_LIBRARY

SOURCES += \
    Note.cpp \
    Pitch.cpp \
    PitchTracker.cpp \
    3rdParty/dywapitchtrack.c \
    PitchMonitor.cpp \
    Utils.cpp

HEADERS +=\
    3rdParty/dywapitchtrack.h \
    Note.h \
    Pitch.h \
    PitchTracker.h \
    Global.h \
    PitchMonitor.h \
    Utils.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
