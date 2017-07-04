#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "Pie.h"
#include "PieSlice.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<Pie>("io.github.witte", 1, 0, "Pie");
    qmlRegisterType<PieSlice>("io.github.witte", 1, 0, "PieSlice");

    engine.load(QUrl(QLatin1String("qrc:/Pomodoro.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
