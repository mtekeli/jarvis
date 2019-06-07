#include "weatherservice.hpp"
#include "jarvisconfig.hpp"
#include "measurement.hpp"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

namespace
{
constexpr auto REQUEST_INTERVAL = 5 * 60 * 1000;

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
    const auto itemDoc = QJsonDocument::fromJson(data);
    const auto rootObject = itemDoc.object();
    const auto mainObject = rootObject.value("main").toObject();
    const auto weatherArr = rootObject.value("weather").toArray();
    const auto windObj = rootObject.value("wind").toObject();
    const auto rainObj = rootObject.value("rain").toObject();

    QJsonObject weatherObject;
    if (weatherArr.count() > 0)
        weatherObject = weatherArr.at(0).toObject();

    const auto temp = mainObject.value("temp").toDouble();

    WeatherInfo info;
    info.temperature = Measurement::parseMeasurement(QString::number(temp));
    info.temp_min = static_cast<int>(mainObject.value("temp_min").toDouble());
    info.temp_max = static_cast<int>(mainObject.value("temp_max").toDouble());
    info.humidity = mainObject.value("humidity").toInt();
    info.pressure = mainObject.value("pressure").toInt();
    info.weather = weatherObject.value("main").toString().toLower();
    info.city = rootObject.value("name").toString();
    info.windSpeed = windObj.value("speed").toDouble();
    info.windDegree = windObj.value("deg").toInt(-1);
    info.rain = rainObj.value("value").toDouble();

    return info;
}

QList<ForecastInfo> parseForecastData(const QByteArray& data)
{
    auto forecasts = QList<ForecastInfo>();
    forecasts.reserve(3);

    const auto itemDoc = QJsonDocument::fromJson(data);
    const auto rootObject = itemDoc.object();
    const auto list = rootObject.value("list").toArray();

    const auto currentDay = QDate::currentDate().day();
    double maxDay1 = 0.0, maxDay2 = 0.0, maxDay3 = 0.0;
    QString weatherDay1, weatherDay2, weatherDay3;
    for (const auto& forecast : list)
    {
        const auto forecastData = forecast.toObject();
        const auto temp =
            forecastData.value("main").toObject().value("temp").toDouble();
        const auto weather = forecastData.value("weather")
                                 .toArray()
                                 .at(0)
                                 .toObject()
                                 .value("main")
                                 .toString()
                                 .toLower();
        const auto dateTime = QDateTime::fromString(
            forecastData.value("dt_txt").toString(), "yyyy-MM-dd HH:mm:ss");
        const auto day = dateTime.date().day();

        if (!dateTime.isValid())
        {
            qWarning() << "failed to parse date from forecast data";
            return {};
        }

        if (currentDay == day)
            continue;

        if (currentDay + 1 == day && temp > maxDay1)
        {
            maxDay1 = temp;
            weatherDay1 = weather;
        } else if (currentDay + 2 == day && temp > maxDay2)
        {
            maxDay2 = temp;
            weatherDay2 = weather;
        } else if (currentDay + 3 == day && temp > maxDay3)
        {
            maxDay3 = temp;
            weatherDay3 = weather;
        } else if (currentDay + 3 < day)
            break;
    }

    const auto forecast1 = ForecastInfo{
        Measurement::parseMeasurement(QString::number(maxDay1)), weatherDay1};
    const auto forecast2 = ForecastInfo{
        Measurement::parseMeasurement(QString::number(maxDay2)), weatherDay2};
    const auto forecast3 = ForecastInfo{
        Measurement::parseMeasurement(QString::number(maxDay3)), weatherDay3};

    return {forecast1, forecast2, forecast3};
}
} // namespace helpers

WeatherService::WeatherService(const QString& countryCode, const QString city,
                               QObject* parent)
    : QObject{parent}, _currentWeatherUrl{currentWeatherUrl(countryCode, city)},
      _forecastUrl{forecastUrl(countryCode, city)}, _currentWeatherAccess{this},
      _forecastAccess{this}, _timer{this}
{
    connect(&_currentWeatherAccess, &QNetworkAccessManager::finished, this,
            &WeatherService::processCurrentWeatherReply);
    connect(&_forecastAccess, &QNetworkAccessManager::finished, this,
            &WeatherService::processForecastReply);
    connect(&_timer, &QTimer::timeout, this,
            &WeatherService::requestCurrentWeather);
    connect(&_timer, &QTimer::timeout, this, &WeatherService::requestForecast);

    _timer.setInterval(REQUEST_INTERVAL);

    requestCurrentWeather();
    requestForecast();
}

void WeatherService::setEnabled(const bool enabled)
{
    if (_enabled == enabled)
        return;

    _enabled = enabled;

    if (_enabled)
    {
        requestCurrentWeather();
        _timer.start();
    } else
        _timer.stop();
}

void WeatherService::requestCurrentWeather()
{
    qDebug() << "requesting current weather";
    _currentWeatherAccess.get(QNetworkRequest{QUrl{_currentWeatherUrl}});
}

void WeatherService::requestForecast()
{
    qDebug() << "requesting forecast";
    _forecastAccess.get(QNetworkRequest{QUrl{_forecastUrl}});
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
        qDebug()
            << QStringLiteral(
                   "error received while processing current weather reply: ")
            << reply->error();
        return;
    }

    const auto code =
        reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if (code != 200)
    {
        qDebug() << QStringLiteral("response for current weather returned ")
                 << code;
        return;
    }

    const auto result = reply->readAll();

    if (result.isEmpty())
    {
        qDebug() << QStringLiteral("no data received for the current weather");
        return;
    }

    qDebug() << QStringLiteral("received current weather data:") << result;

    const auto weatherInfo = helpers::parseCurrentWeatherData(result);
    setCurrentWeather(weatherInfo);
}

void WeatherService::processForecastReply(QNetworkReply* reply)
{
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << QStringLiteral(
                        "error received while processing forecast reply: ")
                 << reply->error();
        return;
    }

    const auto code =
        reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

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

    qDebug() << QStringLiteral("received forecast data:") << result;

    const auto forecastInfo = helpers::parseForecastData(result);
    setForecast(forecastInfo);
}
