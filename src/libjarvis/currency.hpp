#pragma once

#include "measurement.hpp"

#include <QObject>

struct CurrencyInfo
{
    QString currency;
    MeasurementInfo value;
};

class Currency : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currency READ currency NOTIFY currencyChanged)
    Q_PROPERTY(Measurement* value READ value NOTIFY valueChanged)
public:
    Currency(const CurrencyInfo& info, QObject* parent = nullptr);

    QString currency() const { return _currency; }
    Measurement* value() const { return _value; }

signals:
    void currencyChanged(QPrivateSignal);
    void valueChanged(QPrivateSignal);

private:
    void setCurrency(const QString& currency);
    void setValue(const MeasurementInfo& info);

    QString _currency;
    Measurement* _value = nullptr;
};
