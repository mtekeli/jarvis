#include "airquality.hpp"

AirQuality::AirQuality(QObject* parent) : QObject{parent} {}

void AirQuality::setIndex(const int index)
{
    if (_index == index)
        return;

    _index = index;
    emit indexChanged({});
}

void AirQuality::setMainPollutant(const QString mainPollutant)
{
    if (_mainPollutant == mainPollutant)
        return;

    _mainPollutant = std::move(mainPollutant);
    emit mainPollutantChanged({});
}
