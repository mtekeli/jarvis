import QtQuick 2.11
import QtQuick.Window 2.11
//import com.mtekeli.mirror 1.0

Window {
    id: root

    property bool initialized: false

    readonly property var weatherService: App.weatherService
    readonly property var currentWeather: weatherService ? weatherService.currentWeather : null
    readonly property string currentWeatherIcon: currentWeather ? "assets/svg/weather/" + currentWeather.weather + ".svg" : "assets/svg/sun.svg"
    readonly property string roomTemperatureReal: RoomService.temperature ? RoomService.temperature.real : "0"
    readonly property string roomTemperatureDecimals: RoomService.temperature ? RoomService.temperature.decimals.substring(0,1) + "°" : "0"
    readonly property string roomHumidityReal: RoomService.humidity ? RoomService.humidity.real : "0"
    readonly property string roomHumidityDecimals: RoomService.humidity ? RoomService.humidity.decimals.substring(0,1) : "0"
    readonly property string weatherTemperatureReal: currentWeather ? currentWeather.temperature.real : "0"
    readonly property string weatherTemperatureDecimals: currentWeather ? currentWeather.temperature.decimals.substring(0,1) + "°" : "0"

    visible: true
    title: qsTr("Jarvis")
    color: "black"
    width: 800
    height: 480

    FontLoader { id: mainFontRegular; source: "assets/fonts/ClearSans-Regular.ttf" }
    FontLoader { id: mainFontLight; source: "assets/fonts/ClearSans-Thin.ttf" }

    Component.onCompleted: {
        RoomService.start()
        initialized = true
        footer.switchState()
    }

    Timer {
        interval: 1000
        repeat: true
        running: root.initialized
        onTriggered: {
            var date = new Date()
            if (timeView.text.indexOf(":") > -1)
                timeView.text = Qt.formatDateTime(new Date(), "hh mm")
            else
                timeView.text = Qt.formatDateTime(new Date(), "hh:mm")

            dateView.text = Qt.formatDateTime(new Date(), "dd MMM yyyy ddd")
        }
    }

    // date and time
    Item {
        width: parent.width
        height: parent.height / 3

        // current time
        Text {
            id: timeView

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: -20
            font.pointSize: 75
            color: "white"
            font.family: mainFontRegular.name
            text: "-- --"
        }

        // current date
        Text {
            id: dateView

            anchors.top: timeView.bottom
            anchors.topMargin: -10
            anchors.horizontalCenter: timeView.horizontalCenter
            height: contentHeight
            font.pointSize: 35
            color: "white"
            font.family: mainFontLight.name
        }

        // location
        Item {
            anchors.top: dateView.bottom
            anchors.topMargin: 7
            anchors.horizontalCenter: dateView.horizontalCenter
            width: locationIcon.width + city.width
            visible: dateView.contentWidth > 0 && LocationService.city !== "" && LocationService.country !== ""

            Image {
                id: locationIcon

                source: "assets/svg/placeholder.svg"
                width: 14
                height: width
                fillMode: Image.PreserveAspectFit
                mipmap: true
            }

            Text {
                id: city

                anchors.left: locationIcon.right
                anchors.leftMargin: 4
                anchors.bottom: locationIcon.bottom
                anchors.bottomMargin: -5
                verticalAlignment: Text.AlignBottom
                text: LocationService.city + ", " + LocationService.country
                color: "white"
                font.family: mainFontLight.name
                font.pointSize: 18
            }
        }
    }

    // footer
    Item {
        id: footer

        anchors.bottom: parent.bottom
        width: parent.width
        height: 240

        readonly property string state1: "summary"
        readonly property string state2: "forecast"
        readonly property string state3: "currency"

        function switchState() {
            if (state === "" || state === state3)
                footer.state = state1
            else if (state === state1)
                footer.state = state2
            else if (state === state2)
                footer.state = state1
        }

        Timer {
            id: switchTimer

            interval: 5000
            running: root.initialized
            repeat: true
            onTriggered: footer.switchState()
        }

        onStateChanged: console.info("state changed to " + state)

        states: [
            State {
                name: footer.state1
                PropertyChanges {
                    target: summary
                    opacity: 1.0
                    height: parent.height
                }
            },
            State {
                name: footer.state2
                PropertyChanges {
                    target: forecast
                    opacity: 1.0
                    height: parent.height
                }
            },
            State {
                name: footer.state3
                PropertyChanges {
                    target: currency
                    opacity: 1.0
                    height: parent.height
                }
            }
        ]

        transitions: [
            Transition {
                from: ""
                to: footer.state1

                ParallelAnimation {
                    NumberAnimation {
                        target: summary
                        property: "opacity"
                        duration: 1000
                        easing.type: Easing.InOutQuad
                    }
                    NumberAnimation {
                        target: summary
                        property: "height"
                        duration: 500
                        easing.type: Easing.OutSine
                    }
                }
            },
            Transition {
                from: footer.state1
                to: footer.state2

                ParallelAnimation {
                    NumberAnimation {
                        target: summary
                        property: "opacity"
                        duration: 500
                        easing.type: Easing.InOutQuad
                    }
                    NumberAnimation {
                        target: forecast
                        property: "opacity"
                        duration: 1000
                        easing.type: Easing.InOutQuad
                    }
                    NumberAnimation {
                        target: forecast
                        property: "height"
                        duration: 500
                        easing.type: Easing.OutSine
                    }
                }
            },
            Transition {
                from: footer.state2
                to: footer.state3

                ParallelAnimation {
                    NumberAnimation {
                        target: forecast
                        property: "opacity"
                        duration: 500
                        easing.type: Easing.InOutQuad
                    }
                    NumberAnimation {
                        target: currency
                        property: "opacity"
                        duration: 1000
                        easing.type: Easing.InOutQuad
                    }
                    NumberAnimation {
                        target: currency
                        property: "height"
                        duration: 500
                        easing.type: Easing.OutSine
                    }
                }
            },
            Transition {
                from: footer.state2
                to: footer.state1

                ParallelAnimation {
                    NumberAnimation {
                        target: forecast
                        property: "opacity"
                        duration: 500
                        easing.type: Easing.InOutQuad
                    }
                    NumberAnimation {
                        target: summary
                        property: "opacity"
                        duration: 1000
                        easing.type: Easing.InOutQuad
                    }
                    NumberAnimation {
                        target: summary
                        property: "height"
                        duration: 500
                        easing.type: Easing.OutSine
                    }
                }
            }
        ]

        Summary {
            id: summary

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            height: - parent.height
            opacity: 0.0
            spacing: 60
            fontName: mainFontLight.name
            currentWeatherIcon: root.currentWeatherIcon
            roomTemperatureReal: root.roomTemperatureReal
            roomTemperatureDecimals: root.roomTemperatureDecimals
            roomHumidityReal: root.roomHumidityReal
            roomHumidityDecimals: root.roomHumidityDecimals
            weatherTemperatureReal: root.weatherTemperatureReal
            weatherTemperatureDecimals: root.weatherTemperatureDecimals
        }

        Forecast {
            id: forecast

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            height: - parent.height
            opacity: 0.0
            spacing: 20
            currentWeatherIcon: root.currentWeatherIcon
            fontName: mainFontRegular.name
            primaryText: root.weatherTemperatureReal
            secondaryText: root.weatherTemperatureDecimals
            primaryTextSize: 56
            secondaryTextSize: 32

            currentWeather: root.currentWeather
        }

        Row {
            id: currency

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            width: parent.width
            height: - parent.height
            opacity: 0.0

            Text {
                width: 100
                height: width
                text: "TEST"
                color: "white"
                font.family: mainFontRegular.name
                font.pointSize: 60
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        // version
        Text {
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            text: App.name + " v" + App.majorVersion + "." + App.minorVersion
            color: "white"
        }
    }
}
