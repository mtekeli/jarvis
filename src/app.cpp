#include "app.h"
#include "roomservice.h"

#include <QQmlContext>

App::App(int argc, char* argv[]) : QGuiApplication(argc, argv)
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    //qmlRegisterUncreatableType<Measurement>("com.mtekeli.mirror", 1, 0, "Measurement", "Cannot init from QML");
    //qmlRegisterUncreatableType<Measurement>("com.mtekeli.mirror", 1, 0, "Measurement", "Cannot init from QML");
    //qmlRegisterUncreatableType<RoomService>("com.mtekeli.mirror", 1, 0, "RoomService", "Cannot init from QML");

    engine.rootContext()->setContextProperty(QStringLiteral("roomService"), &rs);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
}

App::~App()
{
    qDebug() << "App is closing";
}
