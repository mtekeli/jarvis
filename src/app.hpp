#ifndef APP_H
#define APP_H

#include "appsettings.hpp"
#include "jarvisconfig.hpp"

#include <QGuiApplication>
#include <QQmlApplicationEngine>

class WeatherService;
class LocationService;
class RoomService;

class App : public QGuiApplication
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString majorVersion READ majorVersion CONSTANT)
    Q_PROPERTY(QString minorVersion READ minorVersion CONSTANT)
    Q_PROPERTY(WeatherService* weatherService READ weatherService NOTIFY
                   weatherServiceChanged)

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

signals:
    void weatherServiceChanged(QPrivateSignal);

private:
    QQmlApplicationEngine engine = {this};
    AppSettings* _settings = nullptr;
    RoomService* _rs = nullptr;
    LocationService* _ls = nullptr;
    WeatherService* _ws = nullptr;
};

#endif // APP_H
