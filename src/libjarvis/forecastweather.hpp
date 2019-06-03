#pragma once

#include "currentweather.hpp"

#include <QNetworkAccessManager>
#include <QObject>

struct ForecastInfo
{
    MeasurementInfo temperature;
    QString weather;
};

class ForecastWeather : public QObject
{
    Q_OBJECT
    /*    Q_PROPERTY(
            Measurement* temperature READ temperature NOTIFY temperatureChanged)
        Q_PROPERTY(QString weather READ weather NOTIFY weatherChanged)*/

public:
    ForecastWeather(const ForecastInfo& info, QObject* parent = nullptr);

signals:
    void temperatureChanged(QPrivateSignal);
    void weatherChanged(QPrivateSignal);
};
