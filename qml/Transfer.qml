import QtQuick
import QtQuick.Effects
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Basic

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

    RowLayout{
        id: headerLayout
        anchors{
            top: parent.top
            left: parent.left
            right: parent.right
            margins: 20
        }

        Row{
            spacing: 10

            Text{
                anchors{
                    bottom: parent.bottom
                }

                text: qsTr("iFoR's")
                font.family: robotoBlack.font.family
                font.styleName: robotoBlack.font.styleName
                font.pixelSize: 30

                color: "white"
            }
            Text{
                anchors{
                    bottom: parent.bottom
                    bottomMargin: 3
                }

                text: qsTr("Transfer")
                font.family: robotoRegular.font.family
                font.styleName: robotoRegular.font.styleName
                font.pixelSize: 20

                color: "white"
            }
        }
        Item{
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
        UButton{
            property int buttonSize: 23

            Layout.minimumWidth: buttonSize
            Layout.maximumWidth: buttonSize
            Layout.minimumHeight: buttonSize
            Layout.maximumHeight: buttonSize

            backgroundColor: "transparent"
            iconPressedColor: "white"

            icon.source: "qrc:/assets/icons/settings.svg"
            icon.color: "#404040"
            icon.height: buttonSize
            icon.width: buttonSize

            display: AbstractButton.IconOnly

            onClicked:{
                core.currentPage = 1 // Settings page
            }
        }
    }

    Rectangle{
        anchors{
            left: parent.left
            top: headerLayout.bottom
            right: parent.right
            bottom: footerLayout.top
            topMargin: 20
            leftMargin: 20
            rightMargin: 20
        }

        color: "transparent"
        border{
            color: "#404040"
            width: 1
        }
        radius: 20

        Debug{ visible: false }

        Image{
            id: dropIcon
            visible: false
            source: "qrc:/assets/icons/place_item.svg"
            anchors{
                centerIn: parent
            }

            sourceSize{
                width: 150
                height: 150
            }

            antialiasing: true
            fillMode: Image.PreserveAspectFit
        }
        Text{
            text: "Drop your files"
            color: "#404040"
            font.family: robotoRegular.font.family
            font.styleName: robotoRegular.font.styleName
            font.pixelSize: 20

            anchors{
                horizontalCenter: dropIcon.horizontalCenter
                top: dropIcon.bottom
            }
        }

        MultiEffect{
            source: dropIcon
            anchors.fill: dropIcon
            colorizationColor: "#404040"
            colorization: 1

            Behavior on colorizationColor{
                ColorAnimation{
                    duration: 100
                }
            }
        }
        DropArea{
            anchors.fill: parent
            onDropped:{
                core.filesPathStr = drop.text
            }
        }
    }

    StackLayout{
        id: footerLayout
        currentIndex: 0
        height: 40
        anchors{
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }
        RowLayout{
            Text{
                text: core.message
                color: core.messageColor
                font.family: robotoRegular.font.family
                font.styleName: robotoRegular.font.styleName
                font.pixelSize: 20

                Layout.fillHeight: true
                Layout.fillWidth: true

                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }
}
