import QtQuick 2.9
import QtQuick.Layouts 1.3

import "qrc:/qml/Regovar"
import "qrc:/qml/Framework"

Rectangle
{
    id: root
    property int fileId
    property real dateColWidth: 100
    property string date: ""
    property string filename: ""
    property string icon: ""
    property int indent: 1
    property string status: ""

    property bool isHover: false
    signal clicked(var fileId)

    height: Regovar.theme.font.boxSize.normal
    color: "transparent"
    RowLayout
    {
        spacing: 0
        anchors.fill: parent
        Rectangle
        {
            width: indent * Regovar.theme.font.boxSize.normal
            height: Regovar.theme.font.boxSize.normal
            color: "transparent"
        }
        Text
        {
            Layout.minimumWidth: dateColWidth
            font.pixelSize: Regovar.theme.font.size.normal
            color: isHover ?  Regovar.theme.secondaryColor.back.normal : Regovar.theme.frontColor.disable
            verticalAlignment: Text.AlignVCenter
            text: regovar.formatDate(date)
            elide: Text.ElideRight
            visible: date != ""
        }
        Rectangle
        {
            width: Regovar.theme.font.boxSize.normal
            height: Regovar.theme.font.boxSize.normal
            color: "transparent"
            visible: date != ""
        }
        Text
        {
            Layout.minimumWidth: Regovar.theme.font.boxSize.normal
            font.pixelSize: Regovar.theme.font.size.normal
            font.family: Regovar.theme.icons.name
            color: isHover ?  Regovar.theme.secondaryColor.back.normal : Regovar.theme.frontColor.normal
            verticalAlignment: Text.AlignVCenter
            text: icon
        }

        Text
        {
            Layout.fillWidth: true
            font.pixelSize: Regovar.theme.font.size.normal
            font.family: Regovar.theme.font.family
            color: isHover ?  Regovar.theme.secondaryColor.back.normal : Regovar.theme.frontColor.normal
            verticalAlignment: Text.AlignVCenter
            text: filename + (status != "" ? " (" + status + ")" : "")
        }
    }

    MouseArea
    {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: isHover = true
        onExited: isHover = false
        onClicked: root.clicked(fileId)
    }
}
