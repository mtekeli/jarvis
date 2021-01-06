import QtQuick 2.11
import mtekeli.jarvis 1.0

Row {
    id: root

    property AirQuality currentAirQuality
    property CurrentWeather currentWeather
    property string currentWeatherIcon
    property string currentTempReal
    property string currentTempDecimals

    property string fontName
    property int primaryTextSize: 60
    property int secondaryTextSize: 30
    property int textSize: 24

    property real _airQIndex: currentAirQuality ? getAirQualityIndex(currentAirQuality.index) : -1
    property string _airPollutant: currentAirQuality ? currentAirQuality.mainPollutant : ""

    function getAirQualityIndex(index) {
        if (index >= 0 && index <= 50)
            return 0
        if (index > 50 && index <= 100)
            return 1
        if (index > 100 && index <= 150)
            return 2
        if (index > 150 && index <= 200)
            return 3
        if (index > 200 && index <= 300)
            return 4
        if (index > 300 && index <= 500)
            return 5

        return -1
    }

    Row {
        Column {
            ImageWithOverlay {
                width: 100
                height: width
                colorize: true
                color: "white"
                anchors.horizontalCenter: parent.horizontalCenter
                source: root.currentWeatherIcon
            }

            Meter {
                height: width
                anchors.horizontalCenter: parent.horizontalCenter
                primaryTextSize: 64
                secondaryTextSize: 48
                primaryText: root.currentTempReal
                secondaryText: root.currentTempDecimals
            }
        }

        Column {
            Row {
                spacing: 10

                Text {
                    id: wind

                    anchors.verticalCenter: parent.verticalCenter
                    text: (root.currentWeather ? root.currentWeather.windSpeed : "0") + "bft"
                    color: "white"
                    font.pixelSize: root.textSize
                    font.family: root.fontName
                }

                ImageWithOverlay {
                    id: windDirection

                    width: 30
                    height: width
                    colorize: true
                    color: "white"
                    source: "assets/svg/down-arrow.svg"
                    visible: root.currentWeather && root.currentWeather.windDegree >= 0

                    transform: Rotation{
                        angle: root.currentWeather && root.currentWeather.windDegree > 0 ? root.currentWeather.windDegree : 0
                        origin.x: windDirection.width / 2
                        origin.y: windDirection.height / 2
                    }
                }
            }

            Text {
                id: minTemperature
                text: (root.currentWeather ? root.currentWeather.tempMin + "-" + root.currentWeather.tempMax : "0-0") + "°"
                color: "white"
                font.pixelSize: root.textSize
                font.family: root.fontName
            }

            Text {
                id: humidity
                text: (root.currentWeather ? root.currentWeather.humidity : "0") + "%"
                color: "white"
                font.pixelSize: root.textSize
                font.family: root.fontName
            }

            Text {
                id: pressure
                text: (root.currentWeather ? root.currentWeather.pressure : "0") + "hpa"
                color: "white"
                font.pixelSize: root.textSize
                font.family: root.fontName
            }

            Text {
                id: rain
                text: root.currentWeather ? root.currentWeather.rain : "0"
                color: "white"
                font.pixelSize: root.textSize
                font.family: root.fontName
                visible: text != "0"
            }
        }
    }

    Column {
        ImageWithOverlay {
            id: airQualityIcon

            width: 100
            height: 100
            colorize: true
            color: "white"
            source: "assets/svg/airquality/face-good.svg"
        }

        Text {
            id: airQualityIndex

            horizontalAlignment: Text.horizontalAlignment
            anchors.horizontalCenter: parent.horizontalCenter
            text: root.currentAirQuality ? root.currentAirQuality.index : "0"
            color: "white"
            font.family: root.fontName
            font.pixelSize: root.primaryTextSize
        }

        states: [
            State {
                name: "good"
                when: root._airQIndex === 0
                PropertyChanges {
                    target: airQualityIcon
                    source: "assets/svg/airquality/face-good.svg"
                    color: "green"
                }
                PropertyChanges {
                    target: airQualityIndex
                    color: "green"
                }
            },
            State {
                name: "moderate"
                when: root._airQIndex === 1
                PropertyChanges {
                    target: airQualityIcon
                    source: "assets/svg/airquality/face-moderate.svg"
                    color: "yellow"
                }
                PropertyChanges {
                    target: airQualityIndex
                    color: "yellow"
                }
            },
            State {
                name: "unhealthy-for-sensitive-groups"
                when: root._airQIndex === 2
                PropertyChanges {
                    target: airQualityIcon
                    source: "assets/svg/airquality/face-unhealthy1.svg"
                    color: "orange"
                }
                PropertyChanges {
                    target: airQualityIndex
                    color: "orange"
                }
            },
            State {
                name: "unhealthy"
                when: root._airQIndex === 3
                PropertyChanges {
                    target: airQualityIcon
                    source: "assets/svg/airquality/face-unhealthy2.svg"
                    color: "red"
                }
                PropertyChanges {
                    target: airQualityIndex
                    color: "red"
                }
            },
            State {
                name: "very-unhealthy"
                when: root._airQIndex === 4
                PropertyChanges {
                    target: airQualityIcon
                    source: "assets/svg/airquality/face-very-unhealthy.svg"
                    color: "purple"
                }
                PropertyChanges {
                    target: airQualityIndex
                    color: "purple"
                }
            },
            State {
                name: "hazardous"
                when: root._airQIndex === 5
                PropertyChanges {
                    target: airQualityIcon
                    source: "assets/svg/airquality/face-hazardous.svg"
                    color: "saddlebrown"
                }
                PropertyChanges {
                    target: airQualityIndex
                    color: "saddlebrown"
                }
            }
        ]
    }
}
