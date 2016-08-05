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
    3rdParty/wavfile.cpp \
    Note.cpp \
    Pitch.cpp \
    PitchTracker.cpp \
    3rdParty/dywapitchtrack.c \
    PitchMonitor.cpp \
    3rdParty/AudioUtils.cpp

HEADERS +=\
    3rdParty/dywapitchtrack.h \
    3rdParty/wavfile.h \
    Platform/Utils.h \
    Note.h \
    Pitch.h \
    PitchTracker.h \
    Global.h \
    PitchMonitor.h \
    3rdParty/AudioUtils.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
