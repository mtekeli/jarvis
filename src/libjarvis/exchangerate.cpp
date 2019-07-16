#include "exchangerate.hpp"

ExchangeRate::ExchangeRate(const ExchangeRateInfo& info, QObject* parent)
    : QObject{parent}
{
    setBase(info.base);
    setCurrency1(info.currency1);
    setCurrency2(info.currency2);
    setCurrency3(info.currency3);
}

void ExchangeRate::setBase(const QString& base)
{
    if (_base == base)
        return;

    _base = base;
    emit baseChanged({});
}

void ExchangeRate::setCurrency1(const CurrencyInfo& info)
{
    if (_currency1)
    {
        if (_currency1->value())
        {
            if (_currency1->currency() == info.currency &&
                _currency1->value()->real() == info.value.real &&
                _currency1->value()->decimals() == info.value.decimals)
                return;
        }

        _currency1->deleteLater();
    }

    _currency1 = new Currency{info, this};
    emit currency1Changed({});
}

void ExchangeRate::setCurrency2(const CurrencyInfo& info)
{
    if (_currency2)
    {
        if (_currency2->value())
        {
            if (_currency2->currency() == info.currency &&
                _currency2->value()->real() == info.value.real &&
                _currency2->value()->decimals() == info.value.decimals)
                return;
        }

        _currency2->deleteLater();
    }

    _currency2 = new Currency{info, this};
    emit currency2Changed({});
}

void ExchangeRate::setCurrency3(const CurrencyInfo& info)
{
    if (_currency3)
    {
        if (_currency3->value())
        {
            if (_currency3->currency() == info.currency &&
                _currency3->value()->real() == info.value.real &&
                _currency3->value()->decimals() == info.value.decimals)
                return;
        }

        _currency3->deleteLater();
    }

    _currency3 = new Currency{info, this};
    emit currency3Changed({});
}
