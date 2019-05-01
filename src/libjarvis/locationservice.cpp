#include "locationservice.hpp"
#include "locationinfo.hpp"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QTimer>

namespace helpers
{
LocationInfo parseLocationReply(const QByteArray& data)
{
    const auto itemDoc = QJsonDocument::fromJson(data);
    const auto rootObject = itemDoc.object();
    const auto currencyObject = rootObject.value("currency").toObject();
    const auto timeZoneObject = rootObject.value("time_zone").toObject();

    LocationInfo locInfo;
    locInfo.city = rootObject.value("city").toString();
    locInfo.country = rootObject.value("country_name").toString();
    locInfo.flag = rootObject.value("emoji_flag").toString();
    locInfo.currency = currencyObject.value("code").toString();
    locInfo.timezone = timeZoneObject.value("name").toString();

    return locInfo;
}
} // namespace helpers

LocationService::LocationService(const QString& apiUrl, QObject* parent)
    : QObject{parent}, _url{apiUrl}
{
    connect(&_net, &QNetworkAccessManager::finished, this,
            &LocationService::processReply);

    scheduleRequest(1000);
}

void LocationService::processReply(QNetworkReply* reply)
{
    const auto result = reply->readAll();
    reply->deleteLater();

    if (result.isEmpty())
    {
        qDebug() << QStringLiteral("no data received from location service");
        scheduleRequest(60000);
        return;
    }

    qDebug() << QStringLiteral("received data:") << result;

    try
    {
        if (reply->error() != QNetworkReply::NoError)
            throw std::invalid_argument(reply->errorString().toStdString());

        if (result.length() == 0)
            throw std::invalid_argument("no room information was returned");

        {
            const auto locationInfo = helpers::parseLocationReply(result);
            setCity(locationInfo.city);
            setCountry(locationInfo.country);
        }
    }
    catch (std::exception& e)
    {
        qDebug() << QStringLiteral("exception occured during location parse:")
                 << e.what();
        scheduleRequest(60000);
        return;
    }
}

void LocationService::scheduleRequest(const int duration)
{
    qDebug() << "scheduling location request after " << duration << "ms";
    QTimer::singleShot(duration, this, [this]() {
        qDebug() << "requesting location info";
        _net.get(QNetworkRequest{QUrl{_url}});
    });
}

void LocationService::setCity(const QString& city)
{
    if (_city == city)
        return;

    _city = city;
    emit cityChanged({});
}

void LocationService::setCountry(const QString& country)
{
    if (_country == country)
        return;

    _country = country;
    emit countryChanged({});
}
