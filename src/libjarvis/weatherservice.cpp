#include "weatherservice.hpp"
#include "jarvisconfig.hpp"
#include "measurement.hpp"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

namespace
{
    constexpr auto REQUEST_INTERVAL        = 5 * 60 * 1000;
    constexpr auto DEFAULT_MIN_TEMPERATURE = 99.9;
    constexpr auto DEFAULT_MAX_TEMPERATURE = 0.0;

    QString currentWeatherUrl(const QString& country, const QString& city)
    {
        return QStringLiteral("http://api.openweathermap.org/data/2.5/"
                              "weather?q=%1,%2&units=metric&appid=%3")
            .arg(city)
            .arg(country)
            .arg(OPEN_WEATHER_API_KEY);
    }

    QString forecastUrl(const QString& country, const QString& city)
    {
        return QStringLiteral("http://api.openweathermap.org/data/2.5/"
                              "forecast?cnt=32&q=%1,%2&units=metric&appid=%3")
            .arg(city)
            .arg(country)
            .arg(OPEN_WEATHER_API_KEY);
    }
} // namespace

namespace helpers
{
    WeatherInfo parseCurrentWeatherData(const QByteArray& data)
    {
        const auto itemDoc    = QJsonDocument::fromJson(data);
        const auto rootObject = itemDoc.object();
        const auto mainObject = rootObject.value("main").toObject();
        const auto weatherArr = rootObject.value("weather").toArray();
        const auto windObj    = rootObject.value("wind").toObject();
        const auto rainObj    = rootObject.value("rain").toObject();

        QJsonObject weatherObject;
        if (weatherArr.count() > 0)
            weatherObject = weatherArr.at(0).toObject();

        const auto temp = mainObject.value("temp").toDouble();

        WeatherInfo info;
        info.temperature = MeasurementHelpers::parseMeasurement(QString::number(temp));
        if (info.temperature.real.isEmpty())
            qWarning() << "failed to extract weather temp from data" << temp << mainObject.value("temp") << mainObject;
        info.temp_min   = static_cast<int>(mainObject.value("temp_min").toDouble());
        info.temp_max   = static_cast<int>(mainObject.value("temp_max").toDouble());
        info.humidity   = mainObject.value("humidity").toInt();
        info.pressure   = mainObject.value("pressure").toInt();
        info.weather    = weatherObject.value("main").toString().toLower();
        info.city       = rootObject.value("name").toString();
        info.windSpeed  = windObj.value("speed").toDouble();
        info.windDegree = windObj.value("deg").toInt(-1);
        info.rain       = rainObj.value("value").toDouble();

        return info;
    }

    QList<ForecastInfo> parseForecastData(const QByteArray& data)
    {
        auto forecasts = QList<ForecastInfo>();
        forecasts.reserve(3);

        const auto itemDoc    = QJsonDocument::fromJson(data);
        const auto rootObject = itemDoc.object();
        const auto list       = rootObject.value("list").toArray();

        double maxTempDay1 = DEFAULT_MAX_TEMPERATURE, maxTempDay2 = DEFAULT_MAX_TEMPERATURE,
               maxTempDay3 = DEFAULT_MAX_TEMPERATURE;
        double minTempDay1 = DEFAULT_MIN_TEMPERATURE, minTempDay2 = DEFAULT_MIN_TEMPERATURE,
               minTempDay3 = DEFAULT_MIN_TEMPERATURE;
        QString minWeatherDay1, minWeatherDay2, minWeatherDay3, maxWeatherDay1, maxWeatherDay2, maxWeatherDay3;
        const auto day0 = QDate::currentDate().day();
        const auto day1 = QDate::currentDate().addDays(1).day();
        const auto day2 = QDate::currentDate().addDays(2).day();
        const auto day3 = QDate::currentDate().addDays(3).day();

        for (const auto& forecast : list)
        {
            const auto forecastData = forecast.toObject();

            const auto dateTime = QDateTime::fromString(forecastData.value("dt_txt").toString(), "yyyy-MM-dd HH:mm:ss");
            if (!dateTime.isValid())
            {
                qWarning() << "failed to parse date from forecast data";
                return {};
            }
            const auto day = dateTime.date().day();
            // skip the current day
            if (day0 == day)
                continue;

            const auto weatherArr = forecastData.value("weather").toArray();
            if (weatherArr.isEmpty())
            {
                qWarning() << "weather information is missing for the date " << dateTime.toString();
                return {};
            }

            const auto weather = weatherArr.at(0).toObject().value("main").toString().toLower();

            const auto temperature = forecastData.value("main").toObject().value("temp").toDouble();

            if (day1 == day)
            {
                if (temperature > maxTempDay1)
                {
                    maxTempDay1    = temperature;
                    maxWeatherDay1 = weather;
                }
                if (temperature < minTempDay1)
                {
                    minTempDay1    = temperature;
                    minWeatherDay1 = weather;
                }
            }
            else if (day2 == day)
            {
                if (temperature > maxTempDay2)
                {
                    maxTempDay2    = temperature;
                    maxWeatherDay2 = weather;
                }
                if (temperature < minTempDay2)
                {
                    minTempDay2    = temperature;
                    minWeatherDay2 = weather;
                }
            }
            else if (day3 == day)
            {
                if (temperature > maxTempDay3)
                {
                    maxTempDay3    = temperature;
                    maxWeatherDay3 = weather;
                }
                if (temperature < minTempDay3)
                {
                    minTempDay3    = temperature;
                    minWeatherDay3 = weather;
                }
            }
            else if (day3 < day)
                break;
        }

        if (qFuzzyCompare(minTempDay1, DEFAULT_MIN_TEMPERATURE))
            minTempDay1 = 0;
        if (qFuzzyCompare(minTempDay2, DEFAULT_MIN_TEMPERATURE))
            minTempDay2 = 0;
        if (qFuzzyCompare(minTempDay3, DEFAULT_MIN_TEMPERATURE))
            minTempDay3 = 0;

        const auto forecast1 =
            ForecastInfo{MeasurementHelpers::parseMeasurement(QString::number(maxTempDay1)), maxWeatherDay1,
                         MeasurementHelpers::parseMeasurement(QString::number(minTempDay1)), minWeatherDay1};
        const auto forecast2 =
            ForecastInfo{MeasurementHelpers::parseMeasurement(QString::number(maxTempDay2)), maxWeatherDay2,
                         MeasurementHelpers::parseMeasurement(QString::number(minTempDay2)), minWeatherDay2};
        const auto forecast3 =
            ForecastInfo{MeasurementHelpers::parseMeasurement(QString::number(maxTempDay3)), maxWeatherDay3,
                         MeasurementHelpers::parseMeasurement(QString::number(minTempDay3)), minWeatherDay3};

        return QList<ForecastInfo>{} << forecast1 << forecast2 << forecast3;
    }
} // namespace helpers

WeatherService::WeatherService(const QString& countryCode, const QString city, QObject* parent)
    : QObject{parent},
      _currentWeatherUrl{currentWeatherUrl(countryCode, city)},
      _forecastUrl{forecastUrl(countryCode, city)},
      _currentWeatherAccess{this},
      _forecastAccess{this},
      _timer{this}
{
    connect(&_currentWeatherAccess, &QNetworkAccessManager::finished, this,
            &WeatherService::processCurrentWeatherReply);
    connect(&_forecastAccess, &QNetworkAccessManager::finished, this, &WeatherService::processForecastReply);
    connect(&_timer, &QTimer::timeout, this, &WeatherService::requestCurrentWeather);
    connect(&_timer, &QTimer::timeout, this, &WeatherService::requestForecast);

    _timer.setInterval(REQUEST_INTERVAL);
}

void WeatherService::setEnabled(const bool enabled)
{
    if (_enabled == enabled)
        return;

    _enabled = enabled;

    if (_enabled)
    {
        requestCurrentWeather();
        requestForecast();
        _timer.start();
    }
    else
        _timer.stop();
}

void WeatherService::requestCurrentWeather()
{
    // qDebug() << "requesting current weather";
    _currentWeatherAccess.get(QNetworkRequest{{_currentWeatherUrl}});
}

void WeatherService::requestForecast()
{
    // qDebug() << "requesting forecast";
    _forecastAccess.get(QNetworkRequest{{_forecastUrl}});
}

void WeatherService::setCurrentWeather(const WeatherInfo& info)
{
    if (_currentWeather)
        _currentWeather->deleteLater();

    _currentWeather = new CurrentWeather{info, this};

    emit currentWeatherChanged({});
}

void WeatherService::setForecast(const QList<ForecastInfo>& info)
{
    for (const auto& f : _forecast)
    {
        auto forecast = f.value<ForecastWeather*>();
        if (forecast)
            forecast->deleteLater();
    }
    _forecast.clear();

    for (const auto& f : info)
        _forecast << qVariantFromValue(new ForecastWeather{f, this});

    emit forecastChanged({});
}

void WeatherService::processCurrentWeatherReply(QNetworkReply* reply)
{
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << QStringLiteral("error received while processing current weather reply: ") << reply->error();
        return;
    }

    const auto code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if (code != 200)
    {
        qDebug() << QStringLiteral("response for current weather returned ") << code;
        return;
    }

    const auto result = reply->readAll();

    if (result.isEmpty())
    {
        qDebug() << QStringLiteral("no data received for the current weather");
        return;
    }

    const auto weatherInfo = helpers::parseCurrentWeatherData(result);
    setCurrentWeather(weatherInfo);
}

void WeatherService::processForecastReply(QNetworkReply* reply)
{
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << QStringLiteral("error received while processing forecast reply: ") << reply->error();
        return;
    }

    const auto code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if (code != 200)
    {
        qDebug() << QStringLiteral("response for forecast returned ") << code;
        return;
    }

    const auto result = reply->readAll();

    if (result.isEmpty())
    {
        qDebug() << QStringLiteral("no data received for the forecast");
        return;
    }

    const auto forecastInfo = helpers::parseForecastData(result);
    setForecast(forecastInfo);
}
