#pragma once

#include <QObject>

class QSettings;

class AppSettings : public QObject
{
    Q_OBJECT

public:
    AppSettings(const QString& organization, const QString& appName,
                QObject* parent = nullptr);

    QString roomServiceUrl() const { return _roomServiceUrl; }
    int roomServiceInterval() const { return _roomServiceInterval; }
    bool useIpLocation() const { return _useIpLocation; }
    QString city() const { return _city; }
    QString countryCode() const { return _countryCode; }

private:
    QSettings* _settings = nullptr;
    QString _roomServiceUrl;
    int _roomServiceInterval;
    bool _useIpLocation;
    QString _city;
    QString _countryCode;

    void resetToDefaults();
    void read();
};
