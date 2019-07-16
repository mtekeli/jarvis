#pragma once

#include <QNetworkAccessManager>
#include <QObject>
#include <QTimer>

class Measurement;
class QNetworkReply;

class RoomService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(
        Measurement* temperature READ temperature NOTIFY temperatureChanged)
    Q_PROPERTY(Measurement* humidity READ humidity NOTIFY humidityChanged)

public:
    explicit RoomService(const QString& apiAddress, const int interval,
                         QObject* parent = nullptr);

    Measurement* temperature() const { return _temp; }
    Measurement* humidity() const { return _hum; }

signals:
    void temperatureChanged(QPrivateSignal);
    void humidityChanged(QPrivateSignal);

private slots:
    void getMeasurements();

private:
    Measurement* _temp = nullptr;
    Measurement* _hum = nullptr;
    QString _address = "";
    int _interval = 0;
    QTimer _apiTimer;
    QNetworkAccessManager _net;

    void setTemperature(Measurement* m);
    void setHumidity(Measurement* m);
    void processReply(QNetworkReply* reply);
};
