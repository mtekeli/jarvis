#include "app.hpp"
#include "jarvis_config.h"
//#include "model/measurement.hpp"

#include <QQmlContext>

App::App(int argc, char* argv[]) : QGuiApplication(argc, argv)
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    qInfo() << QStringLiteral("Jarvis v%1.%2").arg(JARVIS_VERSION_MAJOR).arg(JARVIS_VERSION_MINOR);

    //qmlRegisterUncreatableType<Measurement>("com.mtekeli.mirror", 1, 0, "Measurement", "Cannot init from QML");
    //qmlRegisterUncreatableType<Measurement>("com.mtekeli.mirror", 1, 0, "Measurement", "Cannot init from QML");
    //qmlRegisterUncreatableType<RoomService>("com.mtekeli.mirror", 1, 0, "RoomService", "Cannot init from QML");

    engine.addImportPath("qrc:/");
    engine.rootContext()->setContextProperty(QStringLiteral("roomService"), &_rs);
    engine.load(QUrl{QStringLiteral("qrc:/main.qml")});
}

App::~App()
{
    qDebug() << "App is closing";
}
