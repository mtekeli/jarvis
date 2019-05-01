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
        width: parent.width
        height: parent.height / 3

        // current time
        Text {
            id: timeView

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: -20
            font.pointSize: 90
            color: "#FFFFFF"
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
            color: "#FFFFFF"
            font.family: mainFontLight.name
        }

        // location
        Item {
            anchors.top: dateView.bottom
            anchors.topMargin: 7
            anchors.horizontalCenter: dateView.horizontalCenter
            width: locationIcon.width + city.width
            visible: dateView.contentWidth > 0

            Image {
                id: locationIcon

                source: "assets/svg/placeholder.svg"
                width: 14
                height: width
                fillMode: Image.PreserveAspectFit
            }

            Text {
                id: city

                anchors.left: locationIcon.right
                anchors.leftMargin: 4
                anchors.bottom: locationIcon.bottom
                anchors.bottomMargin: -5
                verticalAlignment: Text.AlignBottom
                text: qsTr("Ankara, Turkey \ud83c\uddf9\ud83c\uddf7") // TODO
                color: "white"
                font.family: mainFontRegular.name
                font.pointSize: 14
            }
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
            DGauge {
                imageSource: "assets/svg/thermometer.svg"
                primaryTextSize: 70
                fontName: mainFontLight.name
                primaryText: initialized && RoomService.temperature ? RoomService.temperature.real : primaryText
                secondaryText: initialized && RoomService.temperature ? RoomService.temperature.decimals.substring(0,1) + "°" : secondaryText
            }

            // humidity
            DGauge {
                imageSource: "assets/svg/humidity.svg"
                primaryTextSize: 70
                fontName: mainFontLight.name
                primaryText: initialized && RoomService.humidity ? RoomService.humidity.real : primaryText
                secondaryText: initialized && RoomService.humidity ? RoomService.humidity.decimals.substring(0,1) : secondaryText
            }

            // forecast (TODO)
            DGauge {
                imageSource: "assets/svg/sun.svg"
                primaryTextSize: 70
                fontName: mainFontLight.name
                primaryText: initialized && RoomService.temperature ? RoomService.temperature.real : primaryText
                secondaryText: initialized && RoomService.temperature ? RoomService.temperature.decimals.substring(0,1) + "°" : secondaryText
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
