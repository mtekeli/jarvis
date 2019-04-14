import QtQuick 2.0

Item {

    property string primaryText : "0"
    property string secondaryText: "0"
    property int primaryTextSize: 56
    property int secondaryTextSize: 36
    property string fontName

    Text {
        id: txtPrimary

        text: primaryText
        color: "#ffffff"
        font.family: fontName
        font.pointSize: primaryTextSize
    }

    Text {
        id: txtSecondary

        anchors.left: txtPrimary.right
        anchors.bottom: txtPrimary.bottom
        anchors.bottomMargin: 5

        text: "." + secondaryText
        color: "#ffffff"
        font.family: fontName
        font.pointSize: secondaryTextSize
    }
}
