import QtQuick 2.11

Row {
    id: root

    property var currentWeather
    property var forecast
    property alias currentWeatherIcon: currentWeatherIcon.source
    property alias primaryText: meter.primaryText
    property alias secondaryText: meter.secondaryText
    property alias primaryTextSize: meter.primaryTextSize
    property alias secondaryTextSize: meter.secondaryTextSize
    property alias fontName: meter.fontName

    property string _stateShowMin: "showMinimum"

    Timer {
        interval: 1500
        repeat: true
        running: root.opacity > 0.8
        onTriggered: state === "" ? state = root._stateShowMin : state = ""
    }

    states: [
        State {
            name: root._stateShowMin
            PropertyChanges {
                target: minForecast
                opacity: 1.0
                y: 0
            }
            PropertyChanges {
                target: maxForecast
                opacity: 0.0
                y: - parent.height / 2
            }
        }
    ]

    transitions: [
        Transition {
            from: ""
            to: root._stateShowMin
            reversible: true

            ParallelAnimation {
                NumberAnimation {
                    target: minForecast
                    property: "opacity"
                    duration: 250
                    easing.type: Easing.InOutQuad
                }
                NumberAnimation {
                    target: minForecast
                    property: "y"
                    duration: 250
                    easing.type: Easing.OutSine
                }
                NumberAnimation {
                    target: maxForecast
                    property: "opacity"
                    duration: 250
                    easing.type: Easing.InOutQuad
                }
                NumberAnimation {
                    target: maxForecast
                    property: "y"
                    duration: 250
                    easing.type: Easing.OutSine
                }
            }
        }
    ]

    Column {
        ImageWithOverlay {
            id: currentWeatherIcon

            width: 100
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

    Item {
        width: childrenRect.width
        height: 200

        Summary {
            id: minForecast

            spacing: 5
            height: parent.height
            y: parent.height / 2
            opacity: 0
            primaryTextSize: 60
            secondaryTextSize: 30
            fontName: root.fontName

            gauge1Icon: "assets/svg/weather/" + root.forecast[0].minWeather
            gauge1Real: root.forecast[0].minTemperature.real
            gauge1Decimals: root.forecast[0].minTemperature.decimals.substring(0,1)
            gauge2Icon: "assets/svg/weather/" + root.forecast[1].minWeather
            gauge2Real: root.forecast[1].minTemperature.real
            gauge2Decimals: oot.forecast[1].minTemperature.decimals.substring(0,1)
            gauge3Icon: "assets/svg/weather/" + root.forecast[2].minWeather
            gauge3Real: root.forecast[2].minTemperature.real
            gauge3Decimals: root.forecast[2].minTemperature.decimals.substring(0,1)
        }

        Summary {
            id: maxForecast

            opacity: 1.0
            height: parent.height
            spacing: 5
            y: 0
            primaryTextSize: 60
            secondaryTextSize: 30
            fontName: root.fontName

            gauge1Icon: "assets/svg/weather/" + root.forecast[0].maxWeather
            gauge1Real: root.forecast[0].maxTemperature.real
            gauge1Decimals: root.forecast[0].maxTemperature.decimals.substring(0,1)
            gauge2Icon: "assets/svg/weather/" + root.forecast[1].maxWeather
            gauge2Real: root.forecast[1].maxTemperature.real
            gauge2Decimals: root.forecast[1].maxTemperature.decimals.substring(0,1)
            gauge3Icon: "assets/svg/weather/" + root.forecast[2].maxWeather
            gauge3Real: root.forecast[2].maxTemperature.real
            gauge3Decimals: root.forecast[2].maxTemperature.decimals.substring(0,1)
        }
    }
}
