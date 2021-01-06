#pragma once

#include "measurement.hpp"

#include <QObject>

struct WeatherInfo
{
    QString city;
    MeasurementInfo temperature;
    int temp_min;
    int temp_max;
    QString weather;
    int humidity;
    int pressure;
    double windSpeed;
    int windDegree;
    double rain;
};

class CurrentWeather : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString city READ city NOTIFY cityChanged)
    Q_PROPERTY(
        Measurement* temperature READ temperature NOTIFY temperatureChanged)
    Q_PROPERTY(int tempMin READ tempMin NOTIFY minTempChanged)
    Q_PROPERTY(int tempMax READ tempMax NOTIFY maxTempChanged)
    Q_PROPERTY(QString weather READ weather NOTIFY weatherChanged)
    Q_PROPERTY(int humidity READ humidity NOTIFY humidityChanged)
    Q_PROPERTY(int pressure READ pressure NOTIFY pressureChanged)
    Q_PROPERTY(double windSpeed READ windSpeed NOTIFY windSpeedChanged)
    Q_PROPERTY(int windDegree READ windDegree NOTIFY windDegreeChanged)
    Q_PROPERTY(double rain READ rain NOTIFY rainChanged)

public:
    CurrentWeather(QObject* parent = nullptr);
    CurrentWeather(const WeatherInfo& info, QObject* parent = nullptr);

    QString city() const { return _city; }
    Measurement* temperature() const { return _temperature; }
    int tempMin() const { return _tempMin; }
    int tempMax() const { return _tempMax; }
    QString weather() const { return _weather; }
    int humidity() const { return _humidity; }
    int pressure() const { return _pressure; }
    double windSpeed() const { return _windSpeed; }
    int windDegree() const { return _windDegree; }
    double rain() const { return _rain; }

signals:
    void cityChanged(QPrivateSignal);
    void temperatureChanged(QPrivateSignal);
    void minTempChanged(QPrivateSignal);
    void maxTempChanged(QPrivateSignal);
    void weatherChanged(QPrivateSignal);
    void humidityChanged(QPrivateSignal);
    void pressureChanged(QPrivateSignal);
    void windSpeedChanged(QPrivateSignal);
    void windDegreeChanged(QPrivateSignal);
    void rainChanged(QPrivateSignal);

private:
    QString _city;
    QString _weather;
    Measurement* _temperature = nullptr;
    int _tempMin;
    int _tempMax;
    double _windSpeed;
    int _windDegree;
    int _humidity;
    int _pressure;
    double _rain;
};
