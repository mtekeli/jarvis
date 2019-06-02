#pragma once

#include "currentweather.hpp"

#include <QNetworkAccessManager>
#include <QObject>
#include <QTimer>

class WeatherService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(CurrentWeather* currentWeather READ currentWeather NOTIFY
                   currentWeatherChanged)
    // Q_PROPERTY(QList<ForecastWeather> forecast READ forecast NOTIFY
    // forecastChanged)

public:
    WeatherService(const QString& url, QObject* parent = nullptr);

    CurrentWeather* currentWeather() const { return _currentWeather; }
    void setEnabled(const bool enabled);

signals:
    void currentWeatherChanged(QPrivateSignal);

private:
    QString _url;
    QNetworkAccessManager _net;
    QTimer _timer;
    CurrentWeather* _currentWeather = nullptr;
    bool _enabled = false;

    void processReply(QNetworkReply* reply);
    void requestCurrentWeather();
    void setCurrentWeather(const WeatherInfo& info);
};
