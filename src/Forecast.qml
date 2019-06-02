import QtQuick 2.11

Row {
    id: root

    property var currentWeather
    property alias currentWeatherIcon: currentWeatherIcon.source
    property alias primaryText: meter.primaryText
    property alias secondaryText: meter.secondaryText
    property alias primaryTextSize: meter.primaryTextSize
    property alias secondaryTextSize: meter.secondaryTextSize
    property alias fontName: meter.fontName

    Column {
        ImageWithOverlay {
            id: currentWeatherIcon

            width: 110
            height: width
            colorize: true
            color: "white"
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Meter {
            id: meter

            height: width
            anchors.horizontalCenter: parent.horizontalCenter
            primaryTextSize: 64
            secondaryTextSize: 48
        }
    }

    Column {

        Row {
            spacing: 10
            Text {
                id: wind

                anchors.verticalCenter: parent.verticalCenter
                text: (currentWeather ? currentWeather.windSpeed : "0") + "bft"
                color: "white"
                font.pixelSize: 22
                font.family: fontName
            }

            ImageWithOverlay {
                id: windDirection

                width: 30
                height: width
                colorize: true
                color: "white"
                source: "assets/svg/down-arrow.svg"
                visible: currentWeather && currentWeather.windDegree >= 0

                transform: Rotation{
                    angle: currentWeather && currentWeather.windDegree > 0 ? currentWeather.windDegree : 0
                    origin.x: windDirection.width / 2
                    origin.y: windDirection.height / 2
                }
            }
        }

        Text {
            id: minTemperature
            text: (currentWeather ? currentWeather.tempMin + "-" + currentWeather.tempMax : "0-0") + "°"
            color: "white"
            font.pixelSize: 22
            font.family: fontName
        }

        Text {
            id: humidity
            text: (currentWeather ? currentWeather.humidity : "0") + "%"
            color: "white"
            font.pixelSize: 22
            font.family: fontName
        }

        Text {
            id: pressure
            text: (currentWeather ? currentWeather.pressure : "0") + "hpa"
            color: "white"
            font.pixelSize: 22
            font.family: fontName
        }

        Text {
            id: rain
            text: currentWeather ? currentWeather.rain : "0"
            color: "white"
            font.pixelSize: 22
            font.family: fontName
            visible: text != "0"
        }
    }

    // day 2
    DGauge {
        width: 130
        imageSource: "assets/svg/sun.svg"
        primaryTextSize: 50
        fontName: fontName
        primaryText: "20"
        secondaryText: "4"
    }

    // day 3
    DGauge {
        width: 130
        imageSource: "assets/svg/sun.svg"
        primaryTextSize: 50
        fontName: fontName
        primaryText: "20"
        secondaryText: "4"
    }

    // day 4
    DGauge {
        width: 130
        imageSource: "assets/svg/sun.svg"
        colorize: true
        color: "white"
        primaryTextSize: 50
        fontName: fontName
        primaryText: "20"
        secondaryText: "4"
    }
}
