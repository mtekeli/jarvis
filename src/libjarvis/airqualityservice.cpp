#include "airqualityservice.hpp"
#include "airquality.hpp"
#include "jarvisconfig.hpp"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

namespace
{
    constexpr auto REQUEST_INTERVAL = 5 * 60 * 1000;
    const auto REQUEST_URL = QStringLiteral("https://api.airvisual.com/v2/nearest_city?key=%1").arg(IQAIR_API_KEY);
} // namespace

namespace airQualityHelpers
{
    std::tuple<bool, int, QString> parseAirQualityData(const QByteArray& data)
    {
        const auto itemDoc    = QJsonDocument::fromJson(data);
        const auto rootObject = itemDoc.object();
        const auto status     = rootObject.value("status").toString();
        if (status.compare("success") != 0)
        {
            qDebug() << QStringLiteral("air quality status is not success: ") << status;
            return {};
        }

        const auto pollutionObj = rootObject.value(QStringLiteral("data"))
                                      .toObject()
                                      .value(QStringLiteral("current"))
                                      .toObject()
                                      .value(QStringLiteral("pollution"))
                                      .toObject();
        if (pollutionObj.isEmpty())
        {
            qDebug() << QStringLiteral("air quality responde did not contain pollution info");
            return {};
        }

        const auto index         = pollutionObj.value("aqius").toInt();
        const auto mainPollutant = pollutionObj.value("mainus").toString();

        return std::make_tuple(true, index, mainPollutant);
    }
} // namespace airQualityHelpers

AirQualityService::AirQualityService(QObject* parent) : QObject{parent}
{
    connect(&_timer, &QTimer::timeout, this, &AirQualityService::requestAirQuality);
    connect(&_network, &QNetworkAccessManager::finished, this, &AirQualityService::processAirQualityReply);

    _timer.setInterval(REQUEST_INTERVAL);
    _timer.start();

    requestAirQuality();
}

void AirQualityService::requestAirQuality() { _network.get(QNetworkRequest{{REQUEST_URL}}); }

void AirQualityService::processAirQualityReply(QNetworkReply* reply)
{
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << QStringLiteral("error received while processing air quality reply: ") << reply->error();
        return;
    }

    const auto code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if (code != 200)
    {
        qDebug() << QStringLiteral("response for air quality returned ") << code;
        return;
    }

    const auto result = reply->readAll();

    if (result.isEmpty())
    {
        qDebug() << QStringLiteral("no data received for the air quality request");
        return;
    }

    const auto airQuality = airQualityHelpers::parseAirQualityData(result);
    if (const auto r = std::get<0>(airQuality) == false)
        return;

    setAirQuality(std::get<1>(airQuality), std::get<2>(airQuality));
}

void AirQualityService::setAirQuality(const int index, QString mainPollutant)
{
    _airQuality.setIndex(index);
    _airQuality.setMainPollutant(mainPollutant);
}
