#include "app.hpp"
#include "libjarvis/measurement.hpp"

#include <QQmlContext>

App::App(int argc, char* argv[]) : QGuiApplication(argc, argv)
{
    QCoreApplication::setOrganizationName(QStringLiteral("mtekeli"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("mtekeli.io"));
    QCoreApplication::setApplicationName(QStringLiteral("jarvis"));
    QCoreApplication::setApplicationVersion(QStringLiteral("%1.%2")
                                                .arg(JARVIS_VERSION_MAJOR)
                                                .arg(JARVIS_VERSION_MINOR));
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // qmlRegisterUncreatableType<Measurement>("com.mtekeli.mirror", 1, 0,
    // "Measurement", "Cannot init from QML");
    // qmlRegisterUncreatableType<Measurement>("com.mtekeli.mirror", 1, 0,
    // "Measurement", "Cannot init from QML");
    // qmlRegisterUncreatableType<RoomService>("com.mtekeli.mirror", 1, 0,
    // "RoomService", "Cannot init from QML");
    // qmlRegisterUncreatableType<Measurement*>("com.mtekeli.mirror", 1, 0,
    // "Measurement*", "Cannot init from QML");
    qRegisterMetaType<Measurement*>("Measurement");

    _settings = new AppSettings{QGuiApplication::organizationName(),
                                QGuiApplication::applicationName(), this};
    _rs = new RoomService{_settings->roomServiceUrl(),
                          _settings->roomServiceInterval(), this};
    _ls = new LocationService{
        QStringLiteral("https://api.ipdata.co/?api-key=%1").arg(IPDATA_API_KEY),
        this};

    engine.addImportPath("qrc:/");
    engine.rootContext()->setContextProperty(QStringLiteral("RoomService"),
                                             _rs);
    engine.rootContext()->setContextProperty(QStringLiteral("LocationService"),
                                             _ls);
    engine.rootContext()->setContextProperty(QStringLiteral("App"), this);
    engine.load(QUrl{QStringLiteral("qrc:/MainWindow.qml")});
}

App::~App() { qDebug() << QStringLiteral("App is closing"); }
