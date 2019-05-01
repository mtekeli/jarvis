#ifndef APP_H
#define APP_H

#include "jarvisconfig.hpp"
#include "libjarvis/locationservice.hpp"
#include "libjarvis/roomservice.hpp"

#include <QGuiApplication>
#include <QQmlApplicationEngine>

class App : public QGuiApplication
{
    Q_OBJECT
    Q_PROPERTY(QString version READ version CONSTANT)

public:
    explicit App(int argc, char* argv[]);
    ~App();

    QString version() const { return _version; }

private:
    QQmlApplicationEngine engine = {this};
    QString _version;
    RoomService _rs{QStringLiteral("http://%1:8080").arg(JARVIS_DEVICE_IP),
                    1000};
    LocationService _ls{QStringLiteral("https://api.ipdata.co/?api-key=%1")
                            .arg(IPDATA_API_KEY)};
};

#endif // APP_H
