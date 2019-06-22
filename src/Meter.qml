import QtQuick 2.0

Item {
    id: root

    property string primaryText : "0"
    property string secondaryText: "0"
    property int primaryTextSize: 56
    property int secondaryTextSize: 32
    property string fontName

    clip: true

    width: txtPrimary.contentWidth + txtSecondary.contentWidth

    states: [
        State {
            name: "gauge"
            when: txtPrimary.font.pixelSize > txtSecondary.font.pixelSize
            PropertyChanges {
                target: txtSecondary
                anchors.bottom: txtPrimary.bottom
                anchors.bottomMargin: 8
            }
        },
        State {
            name: "reversed"
            when: txtPrimary.font.pixelSize < txtSecondary.font.pixelSize
            PropertyChanges {
                target: txtPrimary
                anchors.bottomMargin: 8
                anchors.bottom: txtSecondary.bottom
            }
        }
    ]

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

            text: "." + root.secondaryText
            color: "white"
            font.family: root.fontName
            font.pixelSize: root.secondaryTextSize
        }
    }
}
