import QtQuick

Flickable{
    anchors{
        top: parent.top
        bottom: dropIcon.top
        left: parent.left
        right: parent.right
        margins: parent.radius
    }
    contentY: contentHeight-height
    contentWidth: col.width
    contentHeight: col.height
    clip: true
    Column {
        anchors{
            top: parent.top
            left: parent.left
            right: parent.right
        }
        id: col
        Repeater {
            id: repeater
            model: core.log.split(/[\n]/g);
            Text {
                text: repeater.model[index]
                color: "white"
            }
        }
    }
}
