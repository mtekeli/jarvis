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
    Q_PROPERTY(
        Measurement* temperature READ temperature NOTIFY temperatureChanged)
    Q_PROPERTY(QString weather READ weather NOTIFY weatherChanged)

public:
    ForecastWeather(const ForecastInfo& info, QObject* parent = nullptr);
    ~ForecastWeather();

    Measurement* temperature() const { return _temperature; }
    QString weather() const { return _weather; }

signals:
    void temperatureChanged(QPrivateSignal);
    void weatherChanged(QPrivateSignal);

private:
    void setTemperature(const QString& real, const QString& decimals);
    void setWeather(const QString& weather);

    Measurement* _temperature = nullptr;
    QString _weather;
};
