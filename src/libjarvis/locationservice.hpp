#ifndef LOCATIONSERVICE_H
#define LOCATIONSERVICE_H

#include <QNetworkAccessManager>
#include <QObject>

class QNetworkReply;

class LocationService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString city READ city NOTIFY cityChanged)
    Q_PROPERTY(QString country READ country NOTIFY countryChanged)

public:
    LocationService(const QString& apiUrl, QObject* parent = nullptr);

    QString city() const { return _city; }
    QString country() const { return _country; }

signals:
    void cityChanged(QPrivateSignal);
    void countryChanged(QPrivateSignal);

private slots:
    void setCity(const QString& city);
    void setCountry(const QString& country);

private:
    QString _url;
    QString _city;
    QString _country;
    QNetworkAccessManager _net;

    void processReply(QNetworkReply* reply);
    void scheduleRequest(const int duration);
};
#endif // LOCATIONSERVICE_H
