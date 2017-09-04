import QtQuick 2.5
import QtQuick.Controls 2.0
import "../Framework" as Controls
import "../Regovar"

TextField
{
    id: control
    placeholderText: qsTr("Enter description")
    font.pixelSize: Regovar.theme.font.size.control
    font.family: "monospace"
    color: Regovar.theme.frontColor.normal

    selectByMouse :true

    background: Rectangle
    {
        implicitWidth: 200
        implicitHeight: Regovar.theme.font.size.control * 2
        color: "transparent"

        Rectangle
        {
            color: control.focus ? Regovar.theme.secondaryColor.back.normal : Regovar.theme.primaryColor.back.normal
            width: 1
            height: 5
            anchors.bottom: parent.bottom
            anchors.left: parent.left
        }
        Rectangle
        {
            color: control.focus ? Regovar.theme.secondaryColor.back.normal : Regovar.theme.primaryColor.back.normal
            width: 1
            height: 5
            anchors.bottom: parent.bottom
            anchors.right: parent.right
        }
        Rectangle
        {
            color: control.focus ? Regovar.theme.secondaryColor.back.normal : Regovar.theme.primaryColor.back.normal
            height: 1
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
        }
    }
}
