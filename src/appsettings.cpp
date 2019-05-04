#include "appsettings.hpp"

constexpr auto DEFAULT_ROOM_SERVICE_URL = "http://192.168.1.77:8080";
constexpr auto DEFAULT_ROOM_SERVICE_INTERVAL = 1000;
constexpr auto MIN_ROOM_SERVICE_INTERVAL = 100;

AppSettings::AppSettings(const QString& organization, const QString& appName,
                         QObject* parent)
    : QObject{parent}, _roomServiceUrl{DEFAULT_ROOM_SERVICE_URL},
      _roomServiceInterval{DEFAULT_ROOM_SERVICE_INTERVAL}
{
    _settings = new QSettings{QSettings::IniFormat, QSettings::UserScope,
                              organization, appName, this};

    if (_settings->childGroups().size() == 0)
        resetToDefaults();
    else
        read();
}

void AppSettings::resetToDefaults()
{
    _settings->beginGroup(QStringLiteral("appSettings"));
    _settings->beginGroup(QStringLiteral("roomService"));
    _settings->setValue(QStringLiteral("url"), DEFAULT_ROOM_SERVICE_URL);
    _settings->setValue(QStringLiteral("pollingInterval"),
                        DEFAULT_ROOM_SERVICE_INTERVAL);
    _settings->endGroup();
    _settings->endGroup();
}

void AppSettings::read()
{
    _settings->beginGroup(QStringLiteral("appSettings"));
    _settings->beginGroup(QStringLiteral("roomService"));
    _roomServiceUrl = _settings->value(QStringLiteral("url")).toString();
    _roomServiceInterval =
        std::max(MIN_ROOM_SERVICE_INTERVAL,
                 _settings->value(QStringLiteral("pollingInterval")).toInt());
    _settings->endGroup();
    _settings->endGroup();
}
