#ifndef LOCATIONINFO_H
#define LOCATIONINFO_H

#include <QObject>

struct LocationInfo
{
    QString city;
    QString country;
    QString countryCode;
    QString flag;
    QString currency;
    QString timezone;
};

#endif // LOCATIONINFO_H
