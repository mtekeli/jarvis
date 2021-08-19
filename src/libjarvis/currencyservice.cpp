#include "currencyservice.hpp"
#include "exchangerate.hpp"
#include "jarvisconfig.hpp"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QStringBuilder>

namespace
{
    constexpr auto REQUEST_INTERVAL = 60 * 60 * 1000;
    const QString REQUEST_URL = QStringLiteral("http://data.fixer.io/api/latest?access_key=") % FIXER_EXCHANGES_API_KEY;
} // namespace

namespace helpers
{
    ExchangeRateInfo parseExchangeRates(const QByteArray& data)
    {
        const auto itemDoc    = QJsonDocument::fromJson(data);
        const auto rootObject = itemDoc.object();
        const auto success    = rootObject.value("success").toBool();

        if (!success)
        {
            qWarning() << "exchange rates contain error: " << data;
            return {};
        }

        const auto rates     = rootObject.value("rates").toObject();
        const auto timestamp = rootObject.value("timestamp").toInt();
        const auto base      = rootObject.value("base").toString(); // EUR by default
        const auto rateTRY   = rates.value("TRY").toDouble();
        const auto rateUSD   = rates.value("USD").toDouble();
        const auto rateGBP   = rates.value("GBP").toDouble();

        // qDebug() << "TRY:" << rateTRY << " USD:" << rateUSD << " GBP" << rateGBP;

        ExchangeRateInfo info;
        info.base      = base;
        info.time      = QDateTime::fromSecsSinceEpoch(timestamp);
        info.currency1 = {"TRY", MeasurementHelpers::parseMeasurement(QString::number(rateTRY))};
        info.currency2 = {"USD", MeasurementHelpers::parseMeasurement(QString::number(rateUSD))};
        info.currency3 = {"GBP", MeasurementHelpers::parseMeasurement(QString::number(rateGBP))};
        return info;
    }
} // namespace helpers

CurrencyService::CurrencyService(QObject* parent) : QObject(parent), _refreshTimer{this}, _net{this}
{
    connect(&_net, &QNetworkAccessManager::finished, this, &CurrencyService::processReply);
    connect(&_refreshTimer, &QTimer::timeout, this, &CurrencyService::requestExchangeRates);

    _refreshTimer.setInterval(REQUEST_INTERVAL);
    _refreshTimer.start();

    requestExchangeRates();
}

void CurrencyService::requestExchangeRates()
{
    // qDebug() << "requesting exchange rates";
    _net.get(QNetworkRequest{{REQUEST_URL}});
}

void CurrencyService::setExchangeRates(const ExchangeRateInfo& info)
{
    if (_rates)
        _rates->deleteLater();

    _rates = new ExchangeRate{info, this};
    emit ratesChanged({});
}

void CurrencyService::processReply(QNetworkReply* reply)
{
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << QStringLiteral("error received while processing exchange rates reply: ") << reply->error();
        return;
    }

    const auto code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if (code != 200)
    {
        qDebug() << QStringLiteral("response for exchange rates returned ") << code;
        return;
    }

    const auto result = reply->readAll();

    if (result.isEmpty())
    {
        qDebug() << QStringLiteral("no data received for the exchange rates");
        return;
    }

    const auto info = helpers::parseExchangeRates(result);
    setExchangeRates(info);
}
