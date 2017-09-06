import QtQuick 2.7
import QtQuick.Layouts 1.3

import "../../../../Regovar"
import "../../../../Framework"


Rectangle
{
    id: root
    color: Regovar.theme.backgroundColor.main

    property alias logicalCondition: andCond.checked


    ColumnLayout
    {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        Text
        {
            Layout.fillWidth: true
            text: qsTr("Add a logical group : \"AND\" or \"OR\" that will be applied to sub conditions wrapped by the logical group.")
            wrapMode: Text.WordWrap
        }

        Text
        {
            text: qsTr("Condition")
        }
        Row
        {
            Layout.fillWidth: true
            spacing: 10

            CheckBox
            {
                id: andCond
                text: qsTr("AND")
                checked: true
                onCheckedChanged: orCond.checked = !checked
            }
            CheckBox
            {
                id: orCond
                text: qsTr("OR")
                checked: false
                onCheckedChanged: andCond.checked = !checked
            }
        }

        Rectangle
        {
            color: "transparent"
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }


    Rectangle
    {
        height: 1
        anchors.left: root.left
        anchors.bottom: root.bottom
        anchors.right: root.right
        color: Regovar.theme.boxColor.border
    }
    Rectangle
    {
        width: 1
        anchors.left: root.left
        anchors.bottom: root.bottom
        anchors.top: root.top
        color: Regovar.theme.boxColor.border
    }
    Rectangle
    {
        width: 1
        anchors.right: root.right
        anchors.bottom: root.bottom
        anchors.top: root.top
        color: Regovar.theme.boxColor.border
    }
}
