#include "../libjarvis/measurement.hpp"
#include <QtTest>

class TestMeasurement : public QObject
{
    Q_OBJECT
private slots:
    void parseMeasurement()
    {
        auto value = "";
        QCOMPARE(MeasurementHelpers::parseMeasurement(value), MeasurementInfo("0", "0"));

        value = "0";
        QCOMPARE(MeasurementHelpers::parseMeasurement(value), MeasurementInfo("0", "0"));

        value = "0.0";
        QCOMPARE(MeasurementHelpers::parseMeasurement(value), MeasurementInfo("0", "0"));

        value = "1";
        QCOMPARE(MeasurementHelpers::parseMeasurement(value), MeasurementInfo("1", "0"));

        value = "1.0";
        QCOMPARE(MeasurementHelpers::parseMeasurement(value), MeasurementInfo("1", "0"));

        value = "21.55";
        QCOMPARE(MeasurementHelpers::parseMeasurement(value), MeasurementInfo("21", "55"));

        value = "23";
        QCOMPARE(MeasurementHelpers::parseMeasurement(value), MeasurementInfo("23", "0"));
    }
};

QTEST_GUILESS_MAIN(TestMeasurement)

#include "test_measurement.moc"
