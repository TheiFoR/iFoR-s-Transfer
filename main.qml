import QtQuick
import QtQuick.Layouts

Window {
    width: 300
    height: 500
    visible: true
    title: qsTr("iFoR's Transfer")
    color: "#191919"

    StackLayout{
        currentIndex: core.currentPage
        anchors.fill: parent

        Transfer{}
        Settings{}
    }
}
