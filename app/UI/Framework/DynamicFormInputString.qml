import QtQuick 2.9
import QtQuick.Layouts 1.3
import Regovar.Core 1.0

import "../Regovar"


GridLayout
{
    id: root
    columnSpacing: 10
    rowSpacing: 5
    columns: 2
    rows: 2

    property ToolParameter model

    Text
    {
        elide: Text.ElideRight
        font.pixelSize: Regovar.theme.font.size.normal
        color: Regovar.theme.primaryColor.back.dark
        text: root.model ? root.model.name : "?"
    }

    TextField
    {
        id: input
        Layout.fillWidth: true
    }

    Text
    {
        Layout.row: 1
        Layout.column: 1
        Layout.fillWidth: true
        text: root.model ? root.model.description : "?"
        font.pixelSize: Regovar.theme.font.size.small
        font.italic: true
        color: Regovar.theme.primaryColor.back.normal
        wrapMode: Text.WordWrap
    }
}
