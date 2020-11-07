import QtQuick 2.11
import QtQuick.Window 2.11
//import com.mtekeli.mirror 1.0

Window {
    id: root

    property bool initialized: false

    readonly property var weatherService: App.weatherService
    readonly property var currentWeather: weatherService ? weatherService.currentWeather : null
    readonly property var forecast: weatherService ? weatherService.forecast : null
    readonly property string currentWeatherIcon: currentWeather ? "assets/svg/weather/" + currentWeather.weather + ".svg" : "assets/svg/sun.svg"
    readonly property string roomTemperatureReal: RoomService.temperature ? RoomService.temperature.real : "0"
    readonly property string roomTemperatureDecimals: RoomService.temperature ? RoomService.temperature.decimals.substring(0,1) + "°" : "0"
    readonly property string roomHumidityReal: RoomService.humidity ? RoomService.humidity.real : "0"
    readonly property string roomHumidityDecimals: RoomService.humidity ? RoomService.humidity.decimals.substring(0,1) : "0"
    readonly property string weatherTemperatureReal: currentWeather ? currentWeather.temperature.real : "0"
    readonly property string weatherTemperatureDecimals: currentWeather ? currentWeather.temperature.decimals.substring(0,1) + "°" : "0"

    readonly property var currentRates: CurrencyService ? CurrencyService.rates : null
    readonly property var currency1: currentRates ? currentRates.currency1 : null
    readonly property var currency2: currentRates ? currentRates.currency2 : null
    readonly property var currency3: currentRates ? currentRates.currency3 : null
    readonly property var currency1Value: currency1 ? currency1.value : null
    readonly property var currency2Value: currency2 ? currency2.value : null
    readonly property var currency3Value: currency3 ? currency3.value : null
    readonly property string currency1Icon: "assets/svg/currency/"+ (currency1 && currency1.currency ? currency1.currency : "placeholder")  +".svg"
    readonly property string currency2Icon: "assets/svg/currency/"+ (currency2 && currency2.currency ? currency2.currency : "placeholder")  +".svg"
    readonly property string currency3Icon: "assets/svg/currency/"+ (currency3 && currency3.currency ? currency3.currency : "placeholder")  +".svg"
    readonly property string currency1ValueReal: currency1Value ? currency1Value.real : "0"
    readonly property string currency1ValueDecimals: currency1Value ? currency1Value.decimals.substring(0,2) : "0"
    readonly property string currency2ValueReal: currency2Value ? currency2Value.real : "0"
    readonly property string currency2ValueDecimals: currency2Value ? currency2Value.decimals.substring(0,2) : "0"
    readonly property string currency3ValueReal: currency3Value ? currency3Value.real : "0"
    readonly property string currency3ValueDecimals: currency3Value ? currency3Value.decimals.substring(0,2) : "0"

    visible: true
    title: qsTr("Jarvis")
    color: "black"
    width: 800
    height: 480

    FontLoader { id: mainFontRegular; source: "assets/fonts/ClearSans-Regular.ttf" }
    FontLoader { id: mainFontLight; source: "assets/fonts/ClearSans-Thin.ttf" }

    Component.onCompleted: {
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
                footer.state = state3
        }

        Timer {
            id: switchTimer

            interval: 5000
            running: root.initialized
            repeat: true
            onTriggered: footer.switchState()
        }

        //onStateChanged: console.info("state changed to " + state)

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
                from: footer.state3
                to: footer.state1

                ParallelAnimation {
                    NumberAnimation {
                        target: currency
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
            fontName: mainFontRegular.name
            primaryTextSize: 70
            secondaryTextSize: 35
            gauge3Icon: root.currentWeatherIcon
            gauge1Real: root.roomTemperatureReal
            gauge1Decimals: root.roomTemperatureDecimals
            gauge2Real: root.roomHumidityReal
            gauge2Decimals: root.roomHumidityDecimals
            gauge3Real: root.weatherTemperatureReal
            gauge3Decimals: root.weatherTemperatureDecimals
        }

        Forecast {
            id: forecast

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            height: - parent.height
            opacity: 0.0
            currentWeatherIcon: root.currentWeatherIcon
            primaryText: root.weatherTemperatureReal
            secondaryText: root.weatherTemperatureDecimals
            fontName: mainFontRegular.name
            primaryTextSize: 70
            secondaryTextSize: 35

            currentWeather: root.currentWeather
            forecast: root.forecast
        }

        Summary {
            id: currency

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            height: - parent.height
            opacity: 0.0
            spacing: 60
            fontName: mainFontRegular.name
            primaryTextSize: 50
            secondaryTextSize: 70
            gauge1Real: root.currency1ValueReal
            gauge1Decimals: root.currency1ValueDecimals
            gauge1Icon: root.currency1Icon
            gauge2Real: root.currency2ValueReal
            gauge2Decimals: root.currency2ValueDecimals
            gauge2Icon: root.currency2Icon
            gauge3Real: root.currency3ValueReal
            gauge3Decimals: root.currency3ValueDecimals
            gauge3Icon: root.currency3Icon
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
