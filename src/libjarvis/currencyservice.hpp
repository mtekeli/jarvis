#pragma once

#include "exchangerate.hpp"

#include <QNetworkAccessManager>
#include <QObject>
#include <QTimer>

class QNetworkReply;

class CurrencyService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ExchangeRate* rates READ rates NOTIFY ratesChanged)

public:
    CurrencyService(QObject* parent = nullptr);

    ExchangeRate* rates() const { return _rates; }

signals:
    void ratesChanged(QPrivateSignal);

private:
    QTimer _refreshTimer;
    QNetworkAccessManager _net;
    ExchangeRate* _rates = nullptr;

    void processReply(QNetworkReply* reply);
    void requestExchangeRates();
    void setExchangeRates(const ExchangeRateInfo& info);
};
