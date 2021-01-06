import QtQuick 2.11
import QtGraphicalEffects 1.12

Item {    
    id: root

    property alias source: image.source
    property alias color: overlay.color
    property alias verticalAlignment: image.verticalAlignment
    property alias colorize: overlay.visible

    Image {
        id: image

        width: root.width
        height: width
        sourceSize: Qt.size(width, height)
        verticalAlignment: Image.AlignBottom
        fillMode: Image.PreserveAspectFit
        mipmap: true
    }

    ColorOverlay {
        id: overlay

        anchors.fill: image
        source: image
        visible: false
    }
}
