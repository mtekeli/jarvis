#ifndef APP_H
#define APP_H

#include "appsettings.hpp"
#include "jarvisconfig.hpp"
#include "libjarvis/locationservice.hpp"
#include "libjarvis/roomservice.hpp"

#include <QGuiApplication>
#include <QQmlApplicationEngine>

class App : public QGuiApplication
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString majorVersion READ majorVersion CONSTANT)
    Q_PROPERTY(QString minorVersion READ minorVersion CONSTANT)

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

private:
    QQmlApplicationEngine engine = {this};
    AppSettings settings{QGuiApplication::organizationName(),
                         QGuiApplication::applicationName()};
    RoomService _rs{settings.roomServiceUrl(), settings.roomServiceInterval()};
    LocationService _ls{QStringLiteral("https://api.ipdata.co/?api-key=%1")
                            .arg(IPDATA_API_KEY)};
};

#endif // APP_H
