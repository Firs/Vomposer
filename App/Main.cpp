#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>

#include "PitchMonitor.h"
#include "PitchMonitorModel.h"

int main(int argc, char *argv[])
{
    QGuiApplication App(argc, argv);

    FPitchMonitor Monitor;
    FPitchMonitorModel Model(Monitor);

    QQmlApplicationEngine AppEngine;
    AppEngine.rootContext()->setContextProperty(
                "PitchMonitor", static_cast<QObject*>(&Model));
    AppEngine.load(QUrl(QStringLiteral("qrc:/Main.qml")));


return App.exec();
}
