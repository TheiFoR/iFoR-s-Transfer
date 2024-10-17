import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs

Item{
    id: root
    FontLoader {
        id: robotoRegular
        source: "qrc:/assets/fonts/Roboto/Roboto-Regular.ttf"
    }
    FontLoader {
        id: robotoBlack
        source: "qrc:/assets/fonts/Roboto/Roboto-Black.ttf"
    }

    Column{
        anchors{
            fill: parent
            margins: 20
        }
        UButton{
            property int buttonSize: 23

            width: buttonSize
            height: buttonSize

            backgroundColor: "transparent"
            iconPressedColor: "white"

            icon.source: "qrc:/assets/icons/arrow_left.svg"
            icon.color: "#404040"
            icon.height: buttonSize
            icon.width: buttonSize

            display: AbstractButton.IconOnly

            onClicked:{
                core.currentPage = 0 // Transfer page
            }
        }
        RowLayout{
            id: downloadPathContainer
            anchors{
                left: parent.left
                right: parent.right
            }

            height: 20
            Text{
                text: qsTr("Download path:")

                font.family: robotoRegular.font.family
                font.styleName: robotoRegular.font.styleName
                font.pixelSize: 20

                color: "#404040"
            }
            Rectangle{
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.topMargin: -3
                Layout.bottomMargin: Layout.topMargin

                implicitHeight:  50
                implicitWidth: 100

                radius: (-Layout.topMargin * 2 + downloadPathContainer.height) / 2

                color: "#202020"

                Text{
                    anchors{
                        left: parent.left
                        leftMargin: parent.radius
                        right: parent.right
                        rightMargin: parent.radius
                        verticalCenter: parent.verticalCenter
                    }

                    text: core.downloadPath

                    font.family: robotoRegular.font.family
                    font.styleName: robotoRegular.font.styleName
                    font.pixelSize: 15

                    color: "#404040"

                    clip: true
                }
            }
            UButton{
                property int buttonSize: downloadPathContainer.height

                Layout.minimumWidth: buttonSize
                Layout.maximumWidth: buttonSize
                Layout.minimumHeight: buttonSize
                Layout.maximumHeight: buttonSize

                backgroundColor: "transparent"
                iconPressedColor: "white"

                icon.source: "qrc:/assets/icons/folder.svg"
                icon.color: "#404040"
                icon.height: buttonSize
                icon.width: buttonSize

                display: AbstractButton.IconOnly

                onClicked:{
                    folderDialog.open()
                }
                FolderDialog{
                    id: folderDialog
                    onAccepted:{
                        core.downloadPath = selectedFolder
                    }
                }
            }

        }

        Item{
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
