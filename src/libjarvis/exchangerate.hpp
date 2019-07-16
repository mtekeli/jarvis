#pragma once

#include "currency.hpp"

#include <QDateTime>
#include <QObject>

struct ExchangeRateInfo
{
    QString base;
    QDateTime time;
    CurrencyInfo currency1;
    CurrencyInfo currency2;
    CurrencyInfo currency3;
};

class ExchangeRate : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString base READ base NOTIFY baseChanged)
    Q_PROPERTY(Currency* currency1 READ currency1 NOTIFY currency1Changed)
    Q_PROPERTY(Currency* currency2 READ currency2 NOTIFY currency2Changed)
    Q_PROPERTY(Currency* currency3 READ currency3 NOTIFY currency3Changed)

public:
    ExchangeRate(const ExchangeRateInfo& info, QObject* parent = nullptr);

    QString base() const { return _base; }
    Currency* currency1() const { return _currency1; }
    Currency* currency2() const { return _currency2; }
    Currency* currency3() const { return _currency3; }

signals:
    void baseChanged(QPrivateSignal);
    void currency1Changed(QPrivateSignal);
    void currency2Changed(QPrivateSignal);
    void currency3Changed(QPrivateSignal);

private:
    void setBase(const QString& base);
    void setCurrency1(const CurrencyInfo& info);
    void setCurrency2(const CurrencyInfo& info);
    void setCurrency3(const CurrencyInfo& info);

    QString _base;
    Currency* _currency1 = nullptr;
    Currency* _currency2 = nullptr;
    Currency* _currency3 = nullptr;
};
