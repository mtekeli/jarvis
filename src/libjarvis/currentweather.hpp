#pragma once

#include "measurement.hpp"

#include <QNetworkAccessManager>
#include <QObject>

struct WeatherInfo
{
    QString city;
    float temperature;
    QString weather;
    int humidity;
};

class CurrentWeather : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString city READ city NOTIFY cityChanged)
    Q_PROPERTY(
        Measurement* temperature READ temperature NOTIFY temperatureChanged)
    Q_PROPERTY(QString weather READ weather NOTIFY weatherChanged)
    Q_PROPERTY(int humidity READ humidity NOTIFY humidityChanged)

public:
    CurrentWeather(QObject* parent = nullptr);
    CurrentWeather(const QString& city, const MeasurementInfo& temperature,
                   const QString& weather, const int humidity,
                   QObject* parent = nullptr);

    QString city() const { return _city; }
    Measurement* temperature() const { return _temperature; }
    QString weather() const { return _weather; }
    int humidity() const { return _humidity; }

signals:
    void cityChanged(QPrivateSignal);
    void temperatureChanged(QPrivateSignal);
    void weatherChanged(QPrivateSignal);
    void humidityChanged(QPrivateSignal);

private:
    QString _city;
    QString _weather;
    Measurement* _temperature;
    int _humidity;
};
