#ifndef APP_SETTINGS_H
#define APP_SETTINGS_H

#include <QObject>
#include <QSettings>

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

#endif // APP_SETTINGS_H
