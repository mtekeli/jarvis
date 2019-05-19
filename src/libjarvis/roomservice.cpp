#include "roomservice.hpp"
#include "measurement.hpp"

#include <QNetworkReply>

namespace helpers
{
QPair<MeasurementInfo, MeasurementInfo> parseRoomReply(const QByteArray& value)
{
    const auto results = value.split(',');
    if (results.length() < 2)
        return {};

    const auto m1 = Measurement::parseMeasurement(results[0]);
    const auto m2 = Measurement::parseMeasurement(results[1]);
    return {m1, m2};
}
} // namespace helpers

RoomService::RoomService(const QString& apiAddress, const int interval,
                         QObject* parent)
    : QObject{parent}
{
    setApiAddr(apiAddress);
    setInterval(interval);

    connect(&_net, &QNetworkAccessManager::finished, this,
            &RoomService::processReply);
    connect(&_apiTimer, &QTimer::timeout, this, &RoomService::getMeasurements);
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
    const auto result = reply->readAll();
    reply->deleteLater();

    if (result.isEmpty())
    {
        qDebug() << QStringLiteral("no data received from room service");
        return;
    }

    qDebug() << QStringLiteral("received data:") << result;
    Measurement* temperature = nullptr;
    Measurement* humidity = nullptr;

    try
    {
        if (reply->error() != QNetworkReply::NoError)
        {
            throw std::invalid_argument(reply->errorString().toStdString());
        }

        if (result.length() == 0)
        {
            throw std::invalid_argument("no room information was returned");
        }

        {
            const auto roomInfo = helpers::parseRoomReply(result);

            temperature = new Measurement{roomInfo.first.real,
                                          roomInfo.first.decimals, this};
            humidity = new Measurement{roomInfo.second.real,
                                       roomInfo.second.decimals, this};
        }
    }
    catch (std::exception& e)
    {
        qDebug() << QStringLiteral("exception occured during parse:")
                 << e.what();
    }

    setTemperature(temperature);
    setHumidity(humidity);
}

void RoomService::setApiAddr(const QString& addr)
{
    if (_address == addr)
        return;

    _address = addr;
}

void RoomService::setInterval(int interval)
{
    if (_interval == interval)
        return;

    _interval = interval;
    qDebug() << QStringLiteral("interval changed to ") << _interval;
}

void RoomService::getMeasurements()
{
    if (_address == "")
        return;

    _net.get(QNetworkRequest{QUrl{_address}});
}

void RoomService::start()
{
    qDebug() << QStringLiteral("starting with interval of ") << _interval;
    getMeasurements();
    _apiTimer.start(_interval);
}

void RoomService::stop() { _apiTimer.stop(); }

void RoomService::restart()
{
    stop();
    start();
}
