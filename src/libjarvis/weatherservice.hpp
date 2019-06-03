#pragma once

#include "currentweather.hpp"
#include "forecastweather.hpp"

#include <QNetworkAccessManager>
#include <QObject>
#include <QTimer>

class WeatherService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(CurrentWeather* currentWeather READ currentWeather NOTIFY
                   currentWeatherChanged)
    //    Q_PROPERTY(
    //        QList<ForecastWeather*> forecast READ forecast NOTIFY
    //        forecastChanged)

public:
    WeatherService(const QString& countryCode, const QString city,
                   QObject* parent = nullptr);

    CurrentWeather* currentWeather() const { return _currentWeather; }
    void setEnabled(const bool enabled);

signals:
    void currentWeatherChanged(QPrivateSignal);
    void forecastChanged(QPrivateSignal);

private:
    QString _currentWeatherUrl;
    QString _forecastUrl;
    QString _countryCode;
    QString _city;
    QNetworkAccessManager _currentWeatherAccess;
    QNetworkAccessManager _forecastAccess;
    QTimer _timer;
    CurrentWeather* _currentWeather = nullptr;
    QList<ForecastWeather*> _forecast;
    bool _enabled = false;

    void processCurrentWeatherReply(QNetworkReply* reply);
    void processForecastReply(QNetworkReply* reply);
    void requestCurrentWeather();
    void requestForecast();
    void setCurrentWeather(const WeatherInfo& info);
    void setForecast(const QList<ForecastInfo>& info);
};
