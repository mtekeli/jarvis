#include "currentweather.hpp"

CurrentWeather::CurrentWeather(QObject* parent) : QObject{parent} {}

CurrentWeather::CurrentWeather(const QString& city,
                               const MeasurementInfo& temperature,
                               const QString& weather, const int humidity,
                               QObject* parent)
    : QObject{parent}, _city{city}, _weather{weather}, _humidity{humidity}
{
    _temperature =
        new Measurement{temperature.real, temperature.decimals, this};
}
