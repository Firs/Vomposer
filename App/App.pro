TEMPLATE = app

QT += qml quick multimedia
CONFIG += c++11

SOURCES += \
    Main.cpp \
    PitchMonitorModel.cpp

RESOURCES += \
    Qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Engine/ -lEngine
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Engine/ -lEngined
else:unix: LIBS += -L$$OUT_PWD/../Engine/ -lEngine

ANDROID_EXTRA_LIBS += $$OUT_PWD/../Engine/libEngine.so

INCLUDEPATH += $$PWD/../Engine
DEPENDPATH += $$PWD/../Engine

HEADERS += \
    PitchMonitorModel.h

