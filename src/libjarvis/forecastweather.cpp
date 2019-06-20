#include "forecastweather.hpp"

ForecastWeather::ForecastWeather(const ForecastInfo& info, QObject* parent)
    : QObject{parent}
{
    setMaxTemperature(info.maxTemperature.real, info.maxTemperature.decimals);
    setMaxWeather(info.maxWeather);
    setMinTemperature(info.minTemperature.real, info.minTemperature.decimals);
    setMinWeather(info.minWeather);
}

ForecastWeather::~ForecastWeather()
{
    qDebug() << "deleting forecast weather" << this;
}

void ForecastWeather::setMaxTemperature(const QString& real,
                                        const QString& decimals)
{
    if (_maxTemperature)
    {
        if (_maxTemperature->real() == real &&
            _maxTemperature->decimals() == decimals)
            return;

        _maxTemperature->deleteLater();
    }

    _maxTemperature = new Measurement{real, decimals, this};

    emit maxTemperatureChanged({});
}

void ForecastWeather::setMaxWeather(const QString& weather)
{
    if (_maxWeather == weather)
        return;

    _maxWeather = weather;

    emit maxWeatherChanged({});
}

void ForecastWeather::setMinTemperature(const QString& real,
                                        const QString& decimals)
{
    if (_minTemperature)
    {
        if (_minTemperature->real() == real &&
            _minTemperature->decimals() == decimals)
            return;

        _minTemperature->deleteLater();
    }

    _minTemperature = new Measurement{real, decimals, this};

    emit minTemperatureChanged({});
}

void ForecastWeather::setMinWeather(const QString& weather)
{
    if (_minWeather == weather)
        return;

    _minWeather = weather;

    emit minWeatherChanged({});
}
