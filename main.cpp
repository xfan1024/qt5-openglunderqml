#include <QGuiApplication>
#include <QQmlApplicationEngine>


#include "squircle.h"

static void qmlPrivateTypeInit()
{
    qmlRegisterType<Squircle>("OpenGLUnderQML", 1, 0, "Squircle");
}

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    qmlPrivateTypeInit();
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    engine.load(url);
    return app.exec();
}
