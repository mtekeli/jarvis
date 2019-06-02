#pragma once

#include "currentweather.hpp"

#include <QNetworkAccessManager>
#include <QObject>

class ForecastWeather : public CurrentWeather
{
    Q_OBJECT
    Q_PROPERTY(QString time READ time NOTIFY timeChanged)

public:
    ForecastWeather(QObject* parent = nullptr);
};
