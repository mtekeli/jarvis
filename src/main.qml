import QtQuick 2.11
import QtQuick.Window 2.11
//import com.mtekeli.mirror 1.0

Window {
    property bool initialized: false
    visible: true
    title: qsTr("Jarvis")
    color: "#000000"
    width: 800
    height: 480

    FontLoader { id: mainFontRegular; source: "qrc:/assets/fonts/ClearSans-Regular.ttf" }
    FontLoader { id: mainFontLight; source: "qrc:/assets/fonts/ClearSans-Thin.ttf" }

    Component.onCompleted: {
        RoomService.start()
        initialized = true
    }

    Timer {
        interval: 1000
        repeat: true
        running: initialized
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
        anchors.fill: parent

        // current time
        Text {
            id: timeView
            font.pointSize: 90
            anchors.horizontalCenter: parent.horizontalCenter
            color: "#FFFFFF"
            font.family: mainFontRegular.name
            text: "-- --"
        }

        // current date
        Text {
            id: dateView
            font.pointSize: 35
            anchors.top: timeView.bottom
            anchors.topMargin: -10
            anchors.horizontalCenter: timeView.horizontalCenter
            color: "#FFFFFF"
            font.family: mainFontLight.name
        }
    }

    // footer
    Item {
        width: parent.width
        height: 240
        anchors.bottom: parent.bottom

        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            height: parent.height
            spacing: 60

            // thermometer
            Item {
                width: 160
                height: parent.height

                Image {
                    id: thermometerIcon
                    anchors.horizontalCenter: parent.horizontalCenter
                    source: "assets/svg/thermometer.svg"
                    width: 64
                    height: width
                    fillMode: Image.PreserveAspectFit
                }

                Meter {
                    anchors.top: thermometerIcon.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: parent.height
                    fontName: mainFontLight.name
                    primaryTextSize: 70
                    primaryText: initialized && RoomService.temperature ? RoomService.temperature.real : primaryText
                    secondaryText: initialized && RoomService.temperature ? RoomService.temperature.decimals.substring(0,1) + "°" : secondaryText
                }
            }

            // humidity
            Item {
                width: 160
                height: parent.height

                Item {
                    id: humidityItem

                    anchors.horizontalCenter: parent.horizontalCenter
                    width: 96
                    height: 64

                    Image {
                        id: humidityIcon

                        source: "assets/svg/humidity.svg"
                        width: 64
                        height: width
                        fillMode: Image.PreserveAspectFit
                    }

                    Image {
                        anchors.left: humidityIcon.right
                        anchors.top: humidityIcon.top
                        anchors.leftMargin: -15
                        source: "assets/svg/humidity.svg"
                        width: 32
                        height: width
                        fillMode: Image.PreserveAspectFit
                    }
                }

                Meter {
                    anchors.top: humidityItem.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: parent.height
                    fontName: mainFontLight.name
                    primaryTextSize: 70
                    primaryText: initialized && RoomService.humidity ? RoomService.humidity.real : primaryText
                    secondaryText: initialized && RoomService.humidity ? RoomService.humidity.decimals.substring(0,1) : secondaryText
                }
            }

            // weather forecast
            Item {
                width: 160
                height: parent.height

                Image {
                    id: forecastIcon
                    anchors.horizontalCenter: parent.horizontalCenter
                    source: "assets/svg/sun.svg"
                    width: 64
                    height: width
                    fillMode: Image.PreserveAspectFit
                }

                Meter {
                    anchors.top: forecastIcon.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: parent.height
                    fontName: mainFontLight.name
                    primaryTextSize: 70
                    primaryText: initialized && RoomService.temperature ? RoomService.temperature.real : primaryText
                    secondaryText: initialized && RoomService.temperature ? RoomService.temperature.decimals.substring(0,1) + "°" : secondaryText
                }
            }
        }

        // version
        Text {
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            text: App.version
            color: "#FFFFFF"
        }
    }
}
