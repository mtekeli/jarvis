#ifndef ROOMSERVICE_H
#define ROOMSERVICE_H

#include <QObject>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "measurement.h"

class RoomService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Measurement* temperature READ temperature NOTIFY temperatureChanged)
    Q_PROPERTY(Measurement* humidity READ humidity NOTIFY humidityChanged)
    Q_PROPERTY(QString apiAddr READ apiAddr WRITE setApiAddr)
    Q_PROPERTY(int interval READ interval WRITE setInterval)

public:
    explicit RoomService(const QString& apiAddress, const int interval, QObject* parent = nullptr);

    Measurement* temperature() { return temp; }
    Measurement* humidity() { return hum; }
    QString apiAddr() const { return addr; }
    int interval() { return interv; }

signals:
    void temperatureChanged();
    void humidityChanged();

private:
    Measurement *temp = nullptr;
    Measurement *hum = nullptr;
    QString addr = "";
    int interv = 0;
    QTimer apiTimer;
    QNetworkAccessManager net;

    void setTemperature(Measurement* m);
    void setHumidity(Measurement* m);
    void processReply(QNetworkReply *reply);
    void setApiAddr(const QString& addr);
    void setInterval(int interval);

private slots:
    void getMeasurements();

public slots:
    void start();
    void stop();
    void restart();

};
#endif // ROOMSERVICE_H
