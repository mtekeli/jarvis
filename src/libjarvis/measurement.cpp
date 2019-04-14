#include "measurement.hpp"

#include <QDebug>

Measurement::Measurement(const QString &real, const QString &decimals, QObject *parent) : QObject{parent}, _real(real), _decimals(decimals)
{
    qDebug() << "CREATING MEASUREMENT "<< this;
}

Measurement::~Measurement()
{
    qDebug() << "DELETING MEASUREMENT "<< this;
}


void Measurement::setReal(const QString &value)
{
    _real = value;
    emit realChanged();
}

void Measurement::setDecimals(const QString &value)
{
    _decimals = value;
    emit decimalsChanged();
}
