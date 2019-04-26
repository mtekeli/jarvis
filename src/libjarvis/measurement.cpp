#include "measurement.hpp"

#include <QDebug>

Measurement::Measurement(const QString &real, const QString &decimals, QObject *parent) : QObject{parent}, _real(real), _decimals(decimals)
{
    qDebug() << QStringLiteral("CREATING MEASUREMENT ")<< this;
}

Measurement::~Measurement()
{
    qDebug() << QStringLiteral("DELETING MEASUREMENT ")<< this;
}


void Measurement::setReal(const QString &value)
{
    if (_real == value)
        return;

    _real = value;
    emit realChanged({});
}

void Measurement::setDecimals(const QString &value)
{
    if (_decimals == value)
        return;

    _decimals = value;
    emit decimalsChanged({});
}
