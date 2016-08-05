#-------------------------------------------------
#
# Project created by QtCreator 2016-07-30T10:11:06
#
#-------------------------------------------------

QT       += testlib multimedia

QT       -= gui

TARGET = TestPitchRecognition
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    TestPitch.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Engine/ -lEngine
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Engine/ -lEngined
else:unix: LIBS += -L$$OUT_PWD/../Engine/ -lEngine

INCLUDEPATH += $$PWD/../Engine
DEPENDPATH += $$PWD/../Engine
