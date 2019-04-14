#ifndef APP_H
#define APP_H

#include <QGuiApplication>
#include <QtDebug>
#include <QQmlApplicationEngine>

#include "libjarvis/roomservice.hpp"

class App : public QGuiApplication
{
public:
    explicit App(int argc, char* argv[]);
    ~App();

private:
    QQmlApplicationEngine engine = {this};
    RoomService _rs {"http://192.168.1.77:8080", 1000};
};

#endif // APP_H
