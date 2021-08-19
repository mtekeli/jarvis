import QtQuick 2.11
import mtekeli.jarvis 1.0

Row {
    id: root

    property var forecast
    property string fontName
    property int primaryTextSize: 60
    property int secondaryTextSize: 32

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

    Item {
        width: childrenRect.width
        height: 200

        Summary {
            id: minForecast

            height: parent.height
            y: parent.height / 2
            opacity: 0
            spacing: root.spacing
            primaryTextSize: root.primaryTextSize
            secondaryTextSize: root.secondaryTextSize
            fontName: root.fontName

            gauge1Icon: "assets/svg/weather/" + root.forecast[0].minWeather
            gauge1Real: root.forecast[0].minTemperature.real
            gauge1Decimals: root.forecast[0].minTemperature.decimals.substring(0,1)
            gauge2Icon: "assets/svg/weather/" + root.forecast[1].minWeather
            gauge2Real: root.forecast[1].minTemperature.real
            gauge2Decimals: root.forecast[1].minTemperature.decimals.substring(0,1)
            gauge3Icon: "assets/svg/weather/" + root.forecast[2].minWeather
            gauge3Real: root.forecast[2].minTemperature.real
            gauge3Decimals: root.forecast[2].minTemperature.decimals.substring(0,1)
        }

        Summary {
            id: maxForecast

            height: parent.height
            opacity: 1.0
            spacing: root.spacing
            y: 0
            primaryTextSize: root.primaryTextSize
            secondaryTextSize: root.secondaryTextSize
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
