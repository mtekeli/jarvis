#include "currentweather.hpp"

CurrentWeather::CurrentWeather(QObject* parent) : QObject{parent} {}

CurrentWeather::CurrentWeather(const WeatherInfo& info, QObject* parent)
    : QObject{parent}, _city{info.city}, _weather{info.weather},
      _tempMin{info.temp_min}, _tempMax{info.temp_max},
      _windSpeed{info.windSpeed}, _windDegree{info.windDegree},
      _humidity{info.humidity}, _pressure{info.pressure}, _rain{info.rain}
{
    _temperature =
        new Measurement{info.temperature.real, info.temperature.decimals, this};
}
