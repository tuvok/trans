import QtQuick 2.7
import QtQuick.Controls 2

import "."

Button
{
    id: button
    property url iconSource
    
    Image
    {
        fillMode: Image.PreserveAspectFit
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        width: Style.button.imageWidth
        source: iconSource
    }
    width: Style.button.width
    height: Style.button.height

    background: Rectangle
    {
        border.width: button.activeFocus ? 2 : 1
        border.color: Style.button.border.color
        radius: Style.button.border.radius
        gradient: button.pressed ? Style.button.gradPressed : Style.button.gradNotPressed
    }

    contentItem: Text
    {
        text: button.text
        font: button.font
        opacity: enabled ? 1.0 : 0.3
        color: button.down ? Style.button.textColor : Style.button.textColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }
}