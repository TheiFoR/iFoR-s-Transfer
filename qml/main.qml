import QtQuick
import QtQuick.Layouts

Window {
    width: 400
    height: 670
    visible: true
    color: "#191919"
    title: qsTr("iFoR's Transfer")

    StackLayout{
        anchors.fill: parent
        currentIndex: core.currentPage

        Transfer{}
        Settings{}
    }
}
