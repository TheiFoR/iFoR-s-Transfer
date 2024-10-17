import QtQuick
import QtQuick.Effects
import QtQuick.Controls
import QtQuick.Controls.Basic

Button {
    property string backgroundColor: "#262626"
    property string backgroundPressedColor: backgroundColor

    property string textColor: "white"
    property string textPressedColor: textColor

    property string iconPressedColor: control.icon.color

    property int radius: backgroundItem.height / 2

    id: control

    text: qsTr("UButton")

    background: Rectangle{
        id: backgroundItem
        color: control.down
               ? control.backgroundPressedColor
               : control.backgroundColor
        radius: control.radius

        Behavior on color{
            ColorAnimation{
                duration: 100
            }
        }
    }

    contentItem: Text {
        visible: control.display !== AbstractButton.IconOnly
        text: control.text
        font: control.font
        opacity: control.enabled ? 1.0 : 0.3
        color: control.down
                ? control.textPressedColor
                : control.textColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    Image{
        id: icon
        visible: false
        source: control.icon.source
        anchors{
            verticalCenter: control.verticalCenter
            left: parent.left
            leftMargin: (control.height - sourceSize.width) / 2
        }
        sourceSize{
            width: control.icon.width
            height: control.icon.height
        }

        antialiasing: true
        fillMode: Image.PreserveAspectFit
    }

    MultiEffect{
        visible: control.icon.source !== "" && control.display !== AbstractButton.TextOnly
        source: icon
        anchors.fill: icon
        colorizationColor: control.down ? control.iconPressedColor : control.icon.color
        colorization: 1

        Behavior on colorizationColor{
            ColorAnimation{
                duration: 100
            }
        }
    }
}
