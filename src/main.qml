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

    //
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
            spacing: 80

            Item {
                width: 85
                height: width

                Image {
                    id: thermometerIcon
                    source: "assets/svg/thermometer.svg"
                    width: 64
                    fillMode: Image.PreserveAspectFit
                }

                Meter {
                    anchors.top: thermometerIcon.bottom
                    width: parent.width
                    height: width
                    fontName: mainFontLight.name
                    primaryText: initialized && RoomService.temperature ? RoomService.temperature.real : primaryText
                    primaryTextSize: 70
                    secondaryText: initialized && RoomService.temperature ? RoomService.temperature.decimals.substring(0,1) + "°" : secondaryText
                }
            }

            Item {
                width: 85
                height: width

                Image {
                    id: humidityIcon
                    source: "assets/svg/humidity.svg"
                    width: 64
                    fillMode: Image.PreserveAspectFit
                }

                Meter {
                    anchors.top: humidityIcon.bottom
                    width: parent.width
                    height: width
                    fontName: mainFontLight.name
                    primaryTextSize: 70
                    primaryText: initialized && RoomService.humidity ? RoomService.humidity.real : primaryText
                    secondaryText: initialized && RoomService.humidity ? RoomService.humidity.decimals.substring(0,1) + "%" : secondaryText
                }
            }

            Item {
                width: 85
                height: width

                Rectangle {
                    anchors.fill: parent
                    color: "red"
                }

                Image {
                    id: forecastIcon
                    source: "assets/svg/sun.svg"
                    width: 64
                    fillMode: Image.PreserveAspectFit
                }

                Meter {
                    anchors.top: forecastIcon.bottom
                    width: parent.width
                    height: width
                    fontName: mainFontLight.name
                    primaryTextSize: 70
                    primaryText: initialized && RoomService.humidity ? RoomService.humidity.real : primaryText
                    secondaryText: initialized && RoomService.humidity ? RoomService.humidity.decimals.substring(0,1) + "%" : secondaryText
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
