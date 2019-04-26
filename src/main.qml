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

        // temperature
        Meter {
            id: meterTemp
            width: 85
            height: width
            fontName: mainFontLight.name
            primaryText: initialized && RoomService.temperature ? RoomService.temperature.real : primaryText
            primaryTextSize: 70
            secondaryText: initialized && RoomService.temperature ? RoomService.temperature.decimals.substring(0,1) + "°" : secondaryText
        }

        // humidity
        Meter {
            anchors.top: meterTemp.bottom
            anchors.topMargin: 10
            width: meterTemp.width
            fontName: mainFontLight.name
            primaryText: initialized && RoomService.humidity ? RoomService.humidity.real : primaryText
            secondaryText: initialized && RoomService.humidity ? RoomService.humidity.decimals.substring(0,1) + "%" : secondaryText
        }

        // current time
        Text {
            id: timeView
            font.pointSize: 70
            anchors.horizontalCenter: parent.horizontalCenter
            color: "#FFFFFF"
            font.family: mainFontRegular.name
            text: "-- --"
        }

        // current date
        Text {
            id: dateView
            font.pointSize: 25
            anchors.top: timeView.bottom
            anchors.topMargin: -10
            anchors.horizontalCenter: timeView.horizontalCenter
            color: "#FFFFFF"
            font.family: mainFontLight.name
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

