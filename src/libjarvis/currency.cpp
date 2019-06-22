#include "currency.hpp"

Currency::Currency(const CurrencyInfo& info, QObject* parent) : QObject{parent}
{
    setCurrency(info.currency);
    setValue(info.value);
}

void Currency::setCurrency(const QString& currency)
{
    if (_currency == currency)
        return;

    _currency = currency;
    emit currencyChanged({});
}

void Currency::setValue(const MeasurementInfo& info)
{
    if (_value)
    {
        if (_value->real() == info.real && _value->decimals() == info.decimals)
            return;

        _value->deleteLater();
    }

    _value = new Measurement{info.real, info.decimals, this};
    emit valueChanged({});
}
