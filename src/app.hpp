#ifndef APP_H
#define APP_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "libjarvis/roomservice.hpp"

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
    RoomService _rs{QStringLiteral("http://192.168.1.77:8080"), 1000};
};

#endif // APP_H
