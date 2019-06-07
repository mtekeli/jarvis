#include "forecastweather.hpp"

ForecastWeather::ForecastWeather(const ForecastInfo& info, QObject* parent)
    : QObject{parent}
{
    setTemperature(info.temperature.real, info.temperature.decimals);
    setWeather(info.weather);
}

ForecastWeather::~ForecastWeather()
{
    qDebug() << "deleting forecast weather" << this;
}

void ForecastWeather::setTemperature(const QString& real,
                                     const QString& decimals)
{
    if (_temperature)
    {
        if (_temperature->real() == real &&
            _temperature->decimals() == decimals)
            return;

        _temperature->deleteLater();
    }

    _temperature = new Measurement{real, decimals, this};

    emit temperatureChanged({});
}
void ForecastWeather::setWeather(const QString& weather)
{
    if (_weather == weather)
        return;

    _weather = weather;

    emit weatherChanged({});
}
