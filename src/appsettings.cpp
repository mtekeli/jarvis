#include "appsettings.hpp"

constexpr auto DEFAULT_ROOM_SERVICE_URL = "http://192.168.1.77:8080";
constexpr auto DEFAULT_ROOM_SERVICE_INTERVAL = 1000;
constexpr auto MIN_ROOM_SERVICE_INTERVAL = 100;

constexpr auto SETTING_GROUP_1 = "appSettings";
constexpr auto SETTING_GROUP_2 = "roomService";
constexpr auto SETTING_KEY_URL = "url";
constexpr auto SETTING_KEY_POLL_INTERVAL = "pollingInterval";
constexpr auto SETTING_KEY_USE_IP_LOCATION = "useIpLocation";

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
    _settings->beginGroup(SETTING_GROUP_1);
    _settings->setValue(SETTING_KEY_USE_IP_LOCATION, true);
    _settings->beginGroup(SETTING_GROUP_2);
    _settings->setValue(SETTING_KEY_URL, DEFAULT_ROOM_SERVICE_URL);
    _settings->setValue(SETTING_KEY_POLL_INTERVAL,
                        DEFAULT_ROOM_SERVICE_INTERVAL);
    _settings->endGroup();
    _settings->endGroup();
}

void AppSettings::read()
{
    _settings->sync();
    _settings->beginGroup(SETTING_GROUP_1);
    _useIpLocation = _settings->value(SETTING_KEY_USE_IP_LOCATION).toBool();
    _settings->beginGroup(SETTING_GROUP_2);
    _roomServiceUrl = _settings->value(SETTING_KEY_URL).toString();
    _roomServiceInterval =
        std::max(MIN_ROOM_SERVICE_INTERVAL,
                 _settings->value(SETTING_KEY_POLL_INTERVAL).toInt());
    _settings->endGroup();
    _settings->endGroup();
}
