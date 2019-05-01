import QtQuick 2.11

Item {
    property alias imageSource: icon.source
    property alias imageWidth: icon.width
    property alias imageHeight: icon.height
    property alias primaryText: meter.primaryText
    property alias secondaryText: meter.secondaryText
    property alias primaryTextSize: meter.primaryTextSize
    property alias secondaryTextSize: meter.secondaryTextSize
    property alias fontName: meter.fontName

    implicitWidth: 160
    implicitHeight: parent.height

    Image {
        id: icon
        anchors.horizontalCenter: parent.horizontalCenter
        verticalAlignment: Image.AlignBottom
        width: 64
        height: width
        fillMode: Image.PreserveAspectFit
    }

    Meter {
        id: meter
        anchors.top: icon.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        height: parent.height
    }
}
