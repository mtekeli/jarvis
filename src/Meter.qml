import QtQuick 2.0

Item {
    id: root

    property string primaryText : "0"
    property string secondaryText: "0"
    property int primaryTextSize: 56
    property int secondaryTextSize: 32
    property string fontName

    width: txtPrimary.contentWidth + txtSecondary.contentWidth

    Row {

        Text {
            id: txtPrimary

            text: root.primaryText
            color: "white"
            font.family: root.fontName
            font.pixelSize: root.primaryTextSize
        }

        Text {
            id: txtSecondary

            anchors.bottom: txtPrimary.bottom
            anchors.bottomMargin: 8
            text: "." + root.secondaryText
            color: "white"
            font.family: root.fontName
            font.pixelSize: root.secondaryTextSize
        }
    }
}
