import QtQuick 2.0

Row {
    id: root

    property string fontName
    property alias gauge1Icon: gauge1.imageSource
    property alias gauge1Real: gauge1.primaryText
    property alias gauge1Decimals: gauge1.secondaryText
    property alias gauge2Icon: gauge2.imageSource
    property alias gauge2Real: gauge2.primaryText
    property alias gauge2Decimals: gauge2.secondaryText
    property alias gauge3Icon: gauge3.imageSource
    property alias gauge3Real: gauge3.primaryText
    property alias gauge3Decimals: gauge3.secondaryText

    property int primaryTextSize: 60
    property int secondaryTextSize: 32

    DGauge {
        id: gauge1

        imageSource: "assets/svg/thermometer.svg"
        colorize: true
        color: "white"
        primaryTextSize: root.primaryTextSize
        secondaryTextSize: root.secondaryTextSize
        fontName: root.fontName
    }

    DGauge {
        id: gauge2

        imageSource: "assets/svg/humidity.svg"
        colorize: true
        color: "white"
        primaryTextSize: root.primaryTextSize
        secondaryTextSize: root.secondaryTextSize
        fontName: root.fontName
    }

    DGauge {
        id: gauge3

        imageSource: "assets/svg/sun.svg"
        colorize: true
        color: "white"
        primaryTextSize: root.primaryTextSize
        secondaryTextSize: root.secondaryTextSize
        fontName: root.fontName
    }
}
