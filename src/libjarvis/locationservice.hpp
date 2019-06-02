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
    Q_PROPERTY(QString countryCode READ countryCode NOTIFY countryCodeChanged)

public:
    LocationService(const QString& apiUrl, QObject* parent = nullptr);

    QString city() const { return _city; }
    QString country() const { return _country; }
    QString countryCode() const { return _countryCode; }
    void setEnabled(const bool enabled);

signals:
    void cityChanged(QPrivateSignal);
    void countryChanged(QPrivateSignal);
    void countryCodeChanged(QPrivateSignal);
    void locationReceived(QPrivateSignal);

private slots:
    void setCity(const QString& city);
    void setCountry(const QString& country);
    void setCountryCode(const QString& countryCode);

private:
    QString _url;
    QString _city;
    QString _country;
    QString _countryCode;
    QNetworkAccessManager _net;
    bool _enabled = false;

    void processReply(QNetworkReply* reply);
    void scheduleRequest(const int duration);
};
#endif // LOCATIONSERVICE_H
