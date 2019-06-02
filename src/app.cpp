#include "app.hpp"
#include "libjarvis/locationservice.hpp"
#include "libjarvis/measurement.hpp"
#include "libjarvis/roomservice.hpp"
#include "libjarvis/weatherservice.hpp"

#include <QCommandLineParser>
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

    QCommandLineParser parser;
    parser.setApplicationDescription("jarvis helper");
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption enableDevOption("dev", "enable developer profile");
    parser.addOption(enableDevOption);
    parser.process(*this);

    _isDev = parser.isSet(enableDevOption);

    if (_isDev)
        qDebug() << "developer profile is active";

    // qmlRegisterUncreatableType<Measurement>("com.mtekeli.mirror", 1, 0,
    // "Measurement", "Cannot init from QML");
    // qmlRegisterUncreatableType<Measurement>("com.mtekeli.mirror", 1, 0,
    // "Measurement", "Cannot init from QML");
    // qmlRegisterUncreatableType<RoomService>("com.mtekeli.mirror", 1, 0,
    // "RoomService", "Cannot init from QML");
    // qmlRegisterUncreatableType<Measurement*>("com.mtekeli.mirror", 1, 0,
    // "Measurement*", "Cannot init from QML");
    //    qmlRegisterUncreatableType<WeatherService>(
    //        "com.mtekeli.jarvis", 1, 0, "WeatherService", "Cannot init from
    //        QML");
    //    qmlRegisterUncreatableType<CurrentWeather>(
    //        "com.mtekeli.jarvis", 1, 0, "CurrentWeather", "Cannot init from
    //        QML");
    qRegisterMetaType<Measurement*>("Measurement");
    qRegisterMetaType<WeatherService*>("WeatherService");
    qRegisterMetaType<CurrentWeather*>("CurrentWeather");

    _settings = new AppSettings{QGuiApplication::organizationName(),
                                QGuiApplication::applicationName(), this};
    _rs = new RoomService{_settings->roomServiceUrl(),
                          _settings->roomServiceInterval(), this};
    _ls = new LocationService{
        QStringLiteral("https://api.ipdata.co/?api-key=%1").arg(IPDATA_API_KEY),
        this};
    _ls->setEnabled(!_isDev);

    if (_settings->useIpLocation())
        connect(_ls, &LocationService::locationReceived, this, [this]() {
            qDebug() << "location received. Starting weather service based on "
                        "IP location. City:"
                     << _ls->city() << " Country:" << _ls->countryCode();
            _ws = new WeatherService{
                QStringLiteral("http://api.openweathermap.org/data/2.5/"
                               "weather?q=%1,%2&units=metric&appid=%3")
                    .arg(_ls->city())
                    .arg(_ls->countryCode())
                    .arg(OPEN_WEATHER_API_KEY),
                this};
            _ws->setEnabled(!_isDev);
            emit weatherServiceChanged({});
        });
    else
    {
        _ws = new WeatherService{
            QStringLiteral("http://api.openweathermap.org/data/2.5/"
                           "weather?q=%1,%2&units=metric&appid=%3")
                .arg(_settings->city())
                .arg(_settings->countryCode())
                .arg(OPEN_WEATHER_API_KEY),
            this};
        _ws->setEnabled(!_isDev);
    }

    engine.addImportPath("qrc:/");
    engine.rootContext()->setContextProperty(QStringLiteral("RoomService"),
                                             _rs);
    engine.rootContext()->setContextProperty(QStringLiteral("LocationService"),
                                             _ls);
    engine.rootContext()->setContextProperty(QStringLiteral("App"), this);
    engine.load(QUrl{QStringLiteral("qrc:/MainWindow.qml")});
}

App::~App() { qDebug() << QStringLiteral("App is closing"); }
