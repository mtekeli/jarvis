#pragma once

#include "currentweather.hpp"

#include <QNetworkAccessManager>
#include <QObject>

struct ForecastInfo
{
    MeasurementInfo maxTemperature;
    QString maxWeather;
    MeasurementInfo minTemperature;
    QString minWeather;
};

class ForecastWeather : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Measurement* maxTemperature READ maxTemperature NOTIFY
                   maxTemperatureChanged)
    Q_PROPERTY(QString maxWeather READ maxWeather NOTIFY maxWeatherChanged)
    Q_PROPERTY(Measurement* minTemperature READ minTemperature NOTIFY
                   minTemperatureChanged)
    Q_PROPERTY(QString minWeather READ minWeather NOTIFY minWeatherChanged)

public:
    ForecastWeather(const ForecastInfo& info = {}, QObject* parent = nullptr);
    ~ForecastWeather();

    Measurement* maxTemperature() const { return _maxTemperature; }
    QString maxWeather() const { return _maxWeather; }
    Measurement* minTemperature() const { return _minTemperature; }
    QString minWeather() const { return _minWeather; }

signals:
    void maxTemperatureChanged(QPrivateSignal);
    void maxWeatherChanged(QPrivateSignal);
    void minTemperatureChanged(QPrivateSignal);
    void minWeatherChanged(QPrivateSignal);

private:
    void setMaxTemperature(const QString& real, const QString& decimals);
    void setMaxWeather(const QString& weather);
    void setMinTemperature(const QString& real, const QString& decimals);
    void setMinWeather(const QString& weather);

    Measurement* _maxTemperature = nullptr;
    QString _maxWeather;
    Measurement* _minTemperature = nullptr;
    QString _minWeather;
};
