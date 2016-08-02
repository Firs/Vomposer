#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <Engine.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    FEngine Engine;
    Engine.loadFile("Data/Test/C3.wav");
    Engine.startPlayback();

    return app.exec();
}
