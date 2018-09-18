#include "roomservice.h"

namespace helpers {
Measurement* parseMeasurement(const QString& value, QObject* parent) {
    if (!value.contains("."))
        return nullptr;

    const auto figures = value.split('.');
    if (figures.length()!=2)
        return nullptr;;

    return new Measurement{figures[0], figures[1], parent};
}

QList<Measurement*> parseRoomReply(const QByteArray& value, QObject* parent) {
    const auto results = value.split(',');
    if (results.length()<2){
        return {};
    }

    const auto m1 = parseMeasurement(results[0], parent);
    const auto m2 = parseMeasurement(results[1], parent);
    return {m1, m2};
}
}

RoomService::RoomService(const QString &apiAddress, const int interval, QObject *parent): QObject{parent}
{
    setApiAddr(apiAddress);
    setInterval(interval);

    connect(&net, &QNetworkAccessManager::finished, this, &RoomService::processReply);
    connect(&apiTimer, &QTimer::timeout, this, &RoomService::getMeasurements);
}

void RoomService::setTemperature(Measurement *m)
{
    if (temp == m)
        return;

    if (temp) {
        temp->deleteLater();
        temp = nullptr;
    }

    temp = m;
    emit temperatureChanged();
}

void RoomService::setHumidity(Measurement *m)
{
    if (hum == m)
        return;

    if (hum) {
        hum->deleteLater();
        hum = nullptr;
    }

    hum = m;
    emit humidityChanged();
}

void RoomService::processReply(QNetworkReply *reply)
{
    const auto result = reply->readAll();
    reply->deleteLater();
    qDebug() << result;
    QList<Measurement*> roomInfo;
    Measurement* temp;
    Measurement* hum;

    try {
        if (reply->error() != QNetworkReply::NoError) {
            throw std::invalid_argument(reply->errorString().toStdString());
        }

        if (result.length()==0){
            throw std::invalid_argument("no room information was returned");
        }

        roomInfo = helpers::parseRoomReply(result, this);
        if (roomInfo.length() == 0) {
            throw std::invalid_argument("no room information available");
        }

        temp = roomInfo.at(0);
        hum = roomInfo.at(1);
    } catch (std::exception& e) {
        qDebug() << "exception occured during parse:" << e.what();
        temp = new Measurement{"0", "0", this};
        hum = temp;
        return;
    }

    setTemperature(temp);
    setHumidity(hum);
}

void RoomService::setApiAddr(const QString &addr)
{
    this->addr = addr;
}

void RoomService::setInterval(int interval)
{
    this->interv=interval;
    qDebug() << "interval changed to " << this->interval();
}

void RoomService::getMeasurements()
{
    if (addr == "")
        return;

    net.get(QNetworkRequest(QUrl(addr)));
}

void RoomService::start()
{
    qDebug() << "starting with interval of "<< interval();
    getMeasurements();
    apiTimer.start(interval());
}

void RoomService::stop()
{
    apiTimer.stop();
}

void RoomService::restart() {
    stop();
    start();
}
