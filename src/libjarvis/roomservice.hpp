#ifndef ROOMSERVICE_H
#define ROOMSERVICE_H

#include <QObject>
#include <QTimer>
#include <QNetworkAccessManager>

class Measurement;
class QNetworkReply;

class RoomService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Measurement* temperature READ temperature NOTIFY temperatureChanged)
    Q_PROPERTY(Measurement* humidity READ humidity NOTIFY humidityChanged)
    Q_PROPERTY(QString apiAddr READ apiAddr WRITE setApiAddr)
    Q_PROPERTY(int interval READ interval WRITE setInterval)

public:
    explicit RoomService(const QString& apiAddress, const int interval, QObject* parent = nullptr);

    Measurement* temperature() const { return _temp; }
    Measurement* humidity() const { return _hum; }
    QString apiAddr() const { return _address; }
    int interval() const { return _interval; }

signals:
    void temperatureChanged(QPrivateSignal);
    void humidityChanged(QPrivateSignal);

private:
    Measurement* _temp = nullptr;
    Measurement* _hum = nullptr;
    QString _address = "";
    int _interval = 0;
    QTimer _apiTimer;
    QNetworkAccessManager _net;

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
