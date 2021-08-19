#pragma once

#include "airquality.hpp"
#include <QNetworkAccessManager>
#include <QObject>
#include <QTimer>

class AirQualityService : public QObject {
    Q_OBJECT
    Q_PROPERTY(AirQuality* airQuality READ airQuality CONSTANT)

public:
    AirQualityService(QObject* parent = nullptr);

    AirQuality* airQuality() { return &_airQuality; }

private slots:
    void requestAirQuality();
    void processAirQualityReply(QNetworkReply* reply);

private:
    AirQuality _airQuality;
    QNetworkAccessManager _network;
    QTimer _timer;

    void setAirQuality(const int index, QString mainPollutant);
};
