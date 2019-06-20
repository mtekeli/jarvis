import QtQuick 2.11
import QtGraphicalEffects 1.12

Item {
    id: root

    property alias imageSource: icon.source
    property alias imageWidth: icon.width
    property alias imageHeight: icon.height
    property alias primaryText: meter.primaryText
    property alias secondaryText: meter.secondaryText
    property alias primaryTextSize: meter.primaryTextSize
    property alias secondaryTextSize: meter.secondaryTextSize
    property alias fontName: meter.fontName

    property bool colorize: false
    property string color: "blue"

    implicitWidth: 160
    implicitHeight: parent.height

    ImageWithOverlay {
        id: icon

        width: 80
        height: width
        anchors.horizontalCenter: parent.horizontalCenter
        verticalAlignment: Image.AlignBottom
        colorize: root.colorize
        color: root.color
    }

    Meter {
        id: meter
        anchors.top: icon.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        height: parent.height
    }
}
