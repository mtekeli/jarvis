import QtQuick 2.11
import QtQuick.Window 2.11
//import com.mtekeli.mirror 1.0

Window {
    property bool initialized: false
    visible: true
    title: qsTr("Jarvis")
    color: "black"
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
        id: mainFooter

        anchors.bottom: parent.bottom
        width: parent.width
        height: 240


        Timer {
            id: switchTimer

            property int counter: 1

            interval: 5000
            running: initialized
            repeat: true
            onTriggered: {
                if (counter == 3)
                    counter = 1
                else
                    counter ++
            }
        }

        states: [
            State {
                name: "firstState"
                when: switchTimer.counter === 1 && initialized
                PropertyChanges {
                    target: firstFooter
                    opacity: 1.0
                    height: parent.height
                }
            },
            State {
                name: "secondState"
                when: switchTimer.counter === 2
                PropertyChanges {
                    target: secondFooter
                    opacity: 1.0
                    height: parent.height
                }
            },
            State {
                name: "thirdState"
                when: switchTimer.counter === 3
                PropertyChanges {
                    target: thirdFooter
                    opacity: 1.0
                    height: parent.height
                }
            }
        ]

        Item {
            id: secondFooter


            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            width: parent.width
            height: - parent.height
            opacity: 0.0

            Rectangle {
                anchors.fill: parent
                color: "red"
            }

            Text {
                anchors.fill: parent
                text: "TEST"
                color: "white"
                font.family: mainFontRegular.name
                font.pointSize: 60
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            Behavior on height {
                NumberAnimation {
                    duration: 500
                    easing.type: Easing.OutSine
                }
            }

            Behavior on opacity {
                NumberAnimation {
                    duration: 500
                }
            }
        }

        Item {
            id: thirdFooter

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            width: parent.width
            height: - parent.height
            opacity: 0.0

            Rectangle {
                anchors.fill: parent
                color: "blue"
            }

            Text {
                anchors.fill: parent
                text: "TEST"
                color: "white"
                font.family: mainFontRegular.name
                font.pointSize: 60
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            Behavior on height {
                NumberAnimation {
                    duration: 500
                    easing.type: Easing.OutSine
                }
            }

            Behavior on opacity {
                NumberAnimation {
                    duration: 500
                }
            }
        }

        Row {
            id: firstFooter

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            height: - parent.height
            spacing: 60
            opacity: 0.0

            // thermometer
            DGauge {
                imageSource: "assets/svg/thermometer.svg"
                primaryTextSize: 60
                fontName: mainFontLight.name
                primaryText: RoomService.temperature ? RoomService.temperature.real : primaryText
                secondaryText: RoomService.temperature ? RoomService.temperature.decimals.substring(0,1) + "°" : secondaryText
            }

            // humidity
            DGauge {
                imageSource: "assets/svg/humidity.svg"
                primaryTextSize: 60
                fontName: mainFontLight.name
                primaryText: RoomService.humidity ? RoomService.humidity.real : primaryText
                secondaryText: RoomService.humidity ? RoomService.humidity.decimals.substring(0,1) : secondaryText
            }

            // forecast (TODO)
            DGauge {
                imageSource: "assets/svg/sun.svg"
                primaryTextSize: 60
                fontName: mainFontLight.name
                primaryText: RoomService.temperature ? RoomService.temperature.real : primaryText
                secondaryText: RoomService.temperature ? RoomService.temperature.decimals.substring(0,1) + "°" : secondaryText
            }

            Behavior on height {
                NumberAnimation {
                    duration: 500
                    easing.type: Easing.OutSine
                }
            }

            Behavior on opacity {
                NumberAnimation {
                    duration: 500
                }
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
