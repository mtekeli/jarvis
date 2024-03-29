#include "roomservice.hpp"
#include "measurement.hpp"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

namespace helpers
{
    QPair<MeasurementInfo, MeasurementInfo> parseRoomReply(const QByteArray& value)
    {
        const auto document = QJsonDocument::fromJson(value);
        const auto rootObj  = document.object();
        const auto temp     = rootObj.value("Temp").toString();
        const auto hum      = rootObj.value("Hum").toString();

        if (temp.isEmpty() || hum.isEmpty())
            return {};

        const auto m1 = MeasurementHelpers::parseMeasurement(temp);
        const auto m2 = MeasurementHelpers::parseMeasurement(hum);
        return {m1, m2};
    }
} // namespace helpers

RoomService::RoomService(const QString& apiAddress, const int interval, QObject* parent)
    : QObject{parent}, _address{apiAddress}, _interval{interval}, _apiTimer{this}, _net{this}
{
    _apiTimer.setInterval(_interval);

    connect(&_net, &QNetworkAccessManager::finished, this, &RoomService::processReply);
    connect(&_apiTimer, &QTimer::timeout, this, &RoomService::getMeasurements);

    qDebug() << "starting room service with interval of " << _interval;
    getMeasurements();
    _apiTimer.start();
}

void RoomService::setTemperature(Measurement* m)
{
    if (_temp == m)
        return;

    if (_temp)
    {
        _temp->deleteLater();
        _temp = nullptr;
    }

    _temp = m;
    emit temperatureChanged({});
}

void RoomService::setHumidity(Measurement* m)
{
    if (_hum == m)
        return;

    if (_hum)
    {
        _hum->deleteLater();
        _hum = nullptr;
    }

    _hum = m;
    emit humidityChanged({});
}

void RoomService::processReply(QNetworkReply* reply)
{
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << QStringLiteral("error received from room service: ") << reply->error();
        return;
    }

    const auto code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if (code != 200)
    {
        qDebug() << QStringLiteral("response from room service returned ") << code;
        return;
    }

    const auto result = reply->readAll();

    if (result.isEmpty())
    {
        qDebug() << QStringLiteral("no data received from room service");
        return;
    }

    // qDebug() << QStringLiteral("received data:") << result;
    Measurement* temperature = nullptr;
    Measurement* humidity    = nullptr;

    const auto roomInfo = helpers::parseRoomReply(result);

    temperature = new Measurement{roomInfo.first.real, roomInfo.first.decimals, this};
    humidity    = new Measurement{roomInfo.second.real, roomInfo.second.decimals, this};

    setTemperature(temperature);
    setHumidity(humidity);
}

void RoomService::getMeasurements() { _net.get(QNetworkRequest{QUrl{_address}}); }
