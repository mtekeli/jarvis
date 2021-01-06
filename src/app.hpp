#pragma once

#include "jarvisconfig.hpp"

#include <QGuiApplication>
#include <QQmlApplicationEngine>

class AppSettings;
class WeatherService;
class LocationService;
class RoomService;
class CurrencyService;
class AirQualityService;

class App : public QGuiApplication
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString majorVersion READ majorVersion CONSTANT)
    Q_PROPERTY(QString minorVersion READ minorVersion CONSTANT)
    Q_PROPERTY(WeatherService* weatherService READ weatherService NOTIFY
                   weatherServiceChanged)
    Q_PROPERTY(AirQualityService* airQualityService READ airQualityService CONSTANT)

public:
    explicit App(int argc, char* argv[]);
    ~App();

    QString name() const { return QGuiApplication::applicationName(); }
    QString majorVersion() const
    {
        return QString{"%1"}.arg(JARVIS_VERSION_MAJOR);
    }
    QString minorVersion() const
    {
        return QString{"%1"}.arg(JARVIS_VERSION_MINOR);
    }
    WeatherService* weatherService() const { return _ws; }
    AirQualityService* airQualityService() const { return _as; }

signals:
    void weatherServiceChanged(QPrivateSignal);

private:
    void setApplicationInfo();
    void processOptions();
    void registerComponents();
    void initializeComponents();

    QQmlApplicationEngine engine = {this};
    AppSettings* _settings = nullptr;
    RoomService* _rs = nullptr;
    LocationService* _ls = nullptr;
    WeatherService* _ws = nullptr;
    CurrencyService* _cs = nullptr;
    AirQualityService* _as = nullptr;
    bool _isDev = false;
};
