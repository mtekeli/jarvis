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

private:
    QSettings* _settings;
    QString _roomServiceUrl;
    int _roomServiceInterval;

    void resetToDefaults();
    void read();
};

#endif // APP_SETTINGS_H
