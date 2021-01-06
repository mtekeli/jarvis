#pragma once

#include <QObject>

class AirQuality : public QObject {
    Q_OBJECT
    Q_PROPERTY(int index READ index NOTIFY indexChanged)
    Q_PROPERTY(QString mainPollutant READ mainPollutant NOTIFY mainPollutantChanged)
public:
    AirQuality(QObject* parent = nullptr);

    int index() const { return _index; }
    QString mainPollutant() const { return _mainPollutant; }

    void setIndex(const int index);
    void setMainPollutant(const QString mainPollutant);

signals:
    void indexChanged(QPrivateSignal);
    void mainPollutantChanged(QPrivateSignal);

private:
    int _index = 0;
    QString _mainPollutant;
};
