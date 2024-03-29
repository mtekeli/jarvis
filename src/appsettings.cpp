#include "appsettings.hpp"

#include <QDebug>
#include <QSettings>

constexpr auto DEFAULT_ROOM_SERVICE_URL      = "http://localhost:8080";
constexpr auto DEFAULT_ROOM_SERVICE_INTERVAL = 2000;
constexpr auto MIN_ROOM_SERVICE_INTERVAL     = 2000;
constexpr auto DEFAULT_CITY                  = "thessaloniki";
constexpr auto DEFAULT_COUNTRY_CODE          = "gr";

constexpr auto SETTING_GROUP_1             = "appSettings";
constexpr auto SETTING_GROUP_2             = "roomService";
constexpr auto SETTING_KEY_URL             = "url";
constexpr auto SETTING_KEY_POLL_INTERVAL   = "pollingInterval";
constexpr auto SETTING_KEY_USE_IP_LOCATION = "useIpLocation";
constexpr auto SETTING_KEY_CITY            = "city";
constexpr auto SETTING_KEY_COUNTRY_CODE    = "countryCode";

AppSettings::AppSettings(const QString& organization, const QString& appName, QObject* parent) : QObject{parent}
{
    _settings = new QSettings{QSettings::IniFormat, QSettings::UserScope, organization, appName, this};

    if (_settings->childGroups().size() == 0)
        resetToDefaults();

    read();
}

void AppSettings::resetToDefaults()
{
    qDebug() << "resettings settings to default values";
    _settings->beginGroup(SETTING_GROUP_1);
    _settings->setValue(SETTING_KEY_USE_IP_LOCATION, true);
    _settings->setValue(SETTING_KEY_CITY, DEFAULT_CITY);
    _settings->setValue(SETTING_KEY_COUNTRY_CODE, DEFAULT_COUNTRY_CODE);
    _settings->beginGroup(SETTING_GROUP_2);
    _settings->setValue(SETTING_KEY_URL, DEFAULT_ROOM_SERVICE_URL);
    _settings->setValue(SETTING_KEY_POLL_INTERVAL, DEFAULT_ROOM_SERVICE_INTERVAL);
    _settings->endGroup();
    _settings->endGroup();
}

void AppSettings::read()
{
    qDebug() << "reading settings";
    _settings->sync();
    _settings->beginGroup(SETTING_GROUP_1);
    _useIpLocation = _settings->value(SETTING_KEY_USE_IP_LOCATION).toBool();
    _city          = _settings->value(SETTING_KEY_CITY).toString();
    _countryCode   = _settings->value(SETTING_KEY_COUNTRY_CODE).toString();
    _settings->beginGroup(SETTING_GROUP_2);
    _roomServiceUrl      = _settings->value(SETTING_KEY_URL).toString();
    _roomServiceInterval = std::max(MIN_ROOM_SERVICE_INTERVAL, _settings->value(SETTING_KEY_POLL_INTERVAL).toInt());
    _settings->endGroup();
    _settings->endGroup();
}
