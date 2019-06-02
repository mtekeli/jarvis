#include "weatherservice.hpp"
#include "measurement.hpp"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

namespace
{
constexpr auto REQUEST_INTERVAL = 5 * 60 * 1000;
}

namespace helpers
{
WeatherInfo parseWeatherData(const QByteArray& data)
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
} // namespace helpers

WeatherService::WeatherService(const QString& url, QObject* parent)
    : QObject{parent}, _url{url}, _net{this}, _timer{this}
{
    connect(&_net, &QNetworkAccessManager::finished, this,
            &WeatherService::processReply);
    connect(&_timer, &QTimer::timeout, this,
            &WeatherService::requestCurrentWeather);

    _timer.setInterval(REQUEST_INTERVAL);

    requestCurrentWeather();
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
    _net.get(QNetworkRequest{QUrl{_url}});
}

void WeatherService::setCurrentWeather(const WeatherInfo& info)
{
    if (_currentWeather)
        _currentWeather->deleteLater();

    _currentWeather = new CurrentWeather{info, this};

    emit currentWeatherChanged({});
}

void WeatherService::processReply(QNetworkReply* reply)
{
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << QStringLiteral("error received from weather service: ")
                 << reply->error();
        return;
    }

    const auto code =
        reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if (code != 200)
    {
        qDebug() << QStringLiteral("response from weather service returned ")
                 << code;
        return;
    }

    const auto result = reply->readAll();

    if (result.isEmpty())
    {
        qDebug() << QStringLiteral("no data received from weather service");
        return;
    }

    qDebug() << QStringLiteral("received weather data:") << result;

    const auto weatherInfo = helpers::parseWeatherData(result);
    setCurrentWeather(weatherInfo);
}
