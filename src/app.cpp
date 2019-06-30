#include "app.hpp"
#include "appsettings.hpp"
#include "libjarvis/currencyservice.hpp"
#include "libjarvis/locationservice.hpp"
#include "libjarvis/measurement.hpp"
#include "libjarvis/roomservice.hpp"
#include "libjarvis/weatherservice.hpp"

#include <QCommandLineParser>
#include <QQmlContext>

App::App(int argc, char* argv[]) : QGuiApplication(argc, argv)
{
    setApplicationInfo();

    processOptions();
    if (_isDev)
        qDebug() << "developer profile is active";

    registerComponents();

    initializeComponents();
}

App::~App() { qDebug() << QStringLiteral("App is closing"); }

void App::setApplicationInfo()
{
    QCoreApplication::setOrganizationName(QStringLiteral("mtekeli"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("mtekeli.io"));
    QCoreApplication::setApplicationName(QStringLiteral("jarvis"));
    QCoreApplication::setApplicationVersion(
        QStringLiteral("%1.%2").arg(majorVersion()).arg(minorVersion()));
}

void App::processOptions()
{
    QCommandLineParser parser;
    parser.setApplicationDescription(QStringLiteral("jarvis helper"));
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption enableDevOption("dev", "enable developer profile");
    parser.addOption(enableDevOption);
    parser.process(*this);

    _isDev = parser.isSet(enableDevOption);
}

void App::registerComponents()
{ // qmlRegisterUncreatableType<Measurement>("com.mtekeli.mirror", 1, 0,
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
    qRegisterMetaType<ForecastWeather*>("Forecast");
    qRegisterMetaType<Currency*>("Currency*");
    qRegisterMetaType<ExchangeRate*>("ExchangeRate*");
    qRegisterMetaType<CurrencyService*>("CurrencyService*");
}

void App::initializeComponents()
{
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
            _ws = new WeatherService{_ls->countryCode(), _ls->city(), this};
            _ws->setEnabled(!_isDev);
            emit weatherServiceChanged({});
        });
    else
    {
        _ws = new WeatherService{_settings->countryCode(), _settings->city(),
                                 this};
        _ws->setEnabled(!_isDev);
    }

    _cs = new CurrencyService{this};

    engine.rootContext()->setContextProperty(QStringLiteral("RoomService"),
                                             _rs);
    engine.rootContext()->setContextProperty(QStringLiteral("LocationService"),
                                             _ls);
    engine.rootContext()->setContextProperty(QStringLiteral("CurrencyService"),
                                             _cs);
    engine.rootContext()->setContextProperty(QStringLiteral("App"), this);
    engine.load(QUrl{QStringLiteral("qrc:/MainWindow.qml")});
}
