#pragma once

#include <QObject>

struct MeasurementInfo
{
    QString real;
    QString decimals;
};

class Measurement : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString real READ real WRITE setReal NOTIFY realChanged)
    Q_PROPERTY(
        QString decimals READ decimals WRITE setDecimals NOTIFY decimalsChanged)

public:
    explicit Measurement(const QString& real, const QString& decimals,
                         QObject* parent = nullptr);
    ~Measurement();
    void setReal(const QString& value);
    void setDecimals(const QString& value);

    static MeasurementInfo parseMeasurement(const QString& value);

signals:
    void realChanged(QPrivateSignal);
    void decimalsChanged(QPrivateSignal);

public slots:
    QString real() const { return _real; }
    QString decimals() const { return _decimals; }

private:
    QString _real;
    QString _decimals;
};
