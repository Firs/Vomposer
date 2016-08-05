#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "PitchMonitor.h"

int main(int argc, char *argv[])
{
    QGuiApplication App(argc, argv);

    QQmlApplicationEngine QmlAppEngine;
    QmlAppEngine.load(QUrl(QStringLiteral("qrc:/Main.qml")));

    FPitchMonitor Monitor;
    Monitor.Start();

    return App.exec();
}
