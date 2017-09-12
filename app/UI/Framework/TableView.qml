import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0

import "../Regovar"

TableView
{
    id: root

    alternatingRowColors: true

    property int rowHeight: Regovar.theme.font.boxSize.control

    style: TableViewStyle
    {
        activateItemOnSingleClick: true
        backgroundColor: Regovar.theme.backgroundColor.main
        alternateBackgroundColor: Regovar.theme.backgroundColor.alt
        highlightedTextColor: Regovar.theme.secondaryColor.front.light
        textColor: Regovar.theme.frontColor.normal
    }



    rowDelegate: Rectangle
    {
        height: root.rowHeight
        color:  styleData.hasActiveFocus ? Regovar.theme.secondaryColor.back.normal :
                (
                    styleData.selected ? Regovar.theme.secondaryColor.back.light :
                    (
                        styleData.alternate ? Regovar.theme.boxColor.back : Regovar.theme.backgroundColor.main
                    )
                )
    }

    headerDelegate: Rectangle
    {
        id: headerRoot
        height: 24
        border.width: 0

        border.color: Regovar.theme.boxColor.border


        LinearGradient
        {
            anchors.fill: parent
            anchors.margins: 1
            start: Qt.point(0, 0)
            end: Qt.point(0, 24)
            gradient: Gradient
            {
                GradientStop { position: 0.0; color: Regovar.theme.boxColor.header1  }
                GradientStop { position: 1.0; color: Regovar.theme.boxColor.header2  }
            }
        }

        // Right border
        Rectangle
        {
            width: 1
            anchors.top: headerRoot.top
            anchors.right: headerRoot.right
            anchors.bottom: headerRoot.bottom
            color: Regovar.theme.boxColor.border
        }
        // Top border
        Rectangle
        {
            height: 1
            anchors.left: headerRoot.left
            anchors.right: headerRoot.right
            anchors.top: headerRoot.top
            color: Regovar.theme.boxColor.border
        }
        // Bottom border
        Rectangle
        {
            height: 1
            anchors.left: headerRoot.left
            anchors.right: headerRoot.right
            anchors.bottom: headerRoot.bottom
            color: Regovar.theme.boxColor.border
        }

        Text
        {
            anchors.fill: parent
            anchors.leftMargin: 5
            anchors.rightMargin: 5
            font.pixelSize: Regovar.theme.font.size.control
            font.family: Regovar.theme.font.familly
            color: Regovar.theme.frontColor.normal
            horizontalAlignment: styleData.textAlignment
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
            text: styleData.value
        }
    }



    // Default delegate for all column
    itemDelegate: Item
    {
        Text
        {
            anchors.leftMargin: 5
            anchors.rightMargin: 5
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: styleData.textAlignment
            font.pixelSize: Regovar.theme.font.size.control
            text: (styleData.value !== undefined && styleData.value !== null) ? styleData.value : "" // + " (" + styleData.row + "," + styleData.column + ")"
            elide: Text.ElideRight
        }
    }
}