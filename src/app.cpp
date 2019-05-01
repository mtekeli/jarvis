#include "app.hpp"
#include "jarvis_config.h"
#include "libjarvis/measurement.hpp"

#include <QQmlContext>

App::App(int argc, char* argv[])
    : QGuiApplication(argc, argv), _version{QStringLiteral("Jarvis v%1.%2")
                                                .arg(JARVIS_VERSION_MAJOR)
                                                .arg(JARVIS_VERSION_MINOR)}
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    qInfo() << _version;

    // qmlRegisterUncreatableType<Measurement>("com.mtekeli.mirror", 1, 0,
    // "Measurement", "Cannot init from QML");
    // qmlRegisterUncreatableType<Measurement>("com.mtekeli.mirror", 1, 0,
    // "Measurement", "Cannot init from QML");
    // qmlRegisterUncreatableType<RoomService>("com.mtekeli.mirror", 1, 0,
    // "RoomService", "Cannot init from QML");
    // qmlRegisterUncreatableType<Measurement*>("com.mtekeli.mirror", 1, 0,
    // "Measurement*", "Cannot init from QML");
    qRegisterMetaType<Measurement*>("Measurement");

    engine.addImportPath("qrc:/");
    engine.rootContext()->setContextProperty(QStringLiteral("RoomService"),
                                             &_rs);
    engine.rootContext()->setContextProperty(QStringLiteral("App"), this);
    engine.load(QUrl{QStringLiteral("qrc:/MainWindow.qml")});
}

App::~App() { qDebug() << "App is closing"; }
