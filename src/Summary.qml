import QtQuick 2.0

Row {
    id: root

    property string fontName
    property string currentWeatherIcon
    property string roomTemperatureReal
    property string roomTemperatureDecimals
    property string roomHumidityReal
    property string roomHumidityDecimals
    property string weatherTemperatureReal
    property string weatherTemperatureDecimals

    // thermometer
    DGauge {
        imageSource: "assets/svg/thermometer.svg"
        primaryTextSize: 60
        fontName: fontName
        primaryText: roomTemperatureReal
        secondaryText: roomTemperatureDecimals
    }

    // humidity
    DGauge {
        imageSource: "assets/svg/humidity.svg"
        primaryTextSize: 60
        fontName: fontName
        primaryText: roomHumidityReal
        secondaryText: roomHumidityDecimals
    }

    // currentWeather
    DGauge {
        imageSource: root.currentWeatherIcon
        colorize: true
        color: "white"
        primaryTextSize: 60
        fontName: fontName
        primaryText: weatherTemperatureReal
        secondaryText: weatherTemperatureDecimals
    }
}
