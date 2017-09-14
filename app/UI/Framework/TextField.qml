import QtQuick 2.7
import QtQuick.Controls 2.0
import "../Framework" as Controls
import "../Regovar"

TextField
{
    id: control
    placeholderText: qsTr("Enter description")
    font.pixelSize: Regovar.theme.font.size.control
    font.family: Regovar.theme.font.familly
    color: Regovar.theme.frontColor.normal
    selectByMouse :true

    background: Rectangle
    {
        implicitWidth: 200
        implicitHeight: Regovar.theme.font.boxSize.control
        border.width: enabled ? 1 : 0
        border.color: control.focus ? Regovar.theme.secondaryColor.back.normal : Regovar.theme.boxColor.border
        color: enabled ? Regovar.theme.boxColor.back : "transparent"
    }
}
