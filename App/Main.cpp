#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>

#include "PitchMonitor.h"
#include "QmlPitchValueSource.h"

int main(int argc, char *argv[])
{
    QGuiApplication App(argc, argv);

    QQmlApplicationEngine AppEngine;

    qmlRegisterType<QmlPitchValueSource>(
                "Vomposer", 1, 0, "PitchValueSource");

    AppEngine.load(QUrl(QStringLiteral("qrc:/Main.qml")));


    return App.exec();
}
