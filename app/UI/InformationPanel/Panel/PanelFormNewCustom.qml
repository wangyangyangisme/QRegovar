import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4

import "qrc:/qml/Regovar"
import "qrc:/qml/Framework"

Rectangle
{
    id: root
    color: Regovar.theme.backgroundColor.main

    property var model
    onModelChanged:
    {

    }

    GridLayout
    {
        anchors.fill: parent
        anchors.margins: 10

        rows: 5
        columns: 2
        columnSpacing: 30
        rowSpacing: 10


        Text
        {
            text: qsTr("Label")
            color: Regovar.theme.primaryColor.back.dark
            font.pixelSize: Regovar.theme.font.size.normal
            font.family: Regovar.theme.font.family
            verticalAlignment: Text.AlignVCenter
            height: 35
        }
        TextField
        {
            id: labelfield
            Layout.fillWidth: true
            placeholder: qsTr("Optional label for the region.")
            onTextChanged: model["label"] = text;
        }


        Text
        {
            text: qsTr("Chr*")
            color: Regovar.theme.primaryColor.back.dark
            font.pixelSize: Regovar.theme.font.size.normal
            font.family: Regovar.theme.font.family
            verticalAlignment: Text.AlignVCenter
            height: 35
            font.bold: true
        }
        TextField
        {
            id: chrfield
            Layout.fillWidth: true
            placeholder: qsTr("The number of the chromosome in the selected ref.")
            onTextChanged: model["chr"] = text;
        }

        Text
        {
            text: qsTr("Start*")
            color: Regovar.theme.primaryColor.back.dark
            font.pixelSize: Regovar.theme.font.size.normal
            font.family: Regovar.theme.font.family
            verticalAlignment: Text.AlignVCenter
            height: 35
            font.bold: true
        }
        TextField
        {
            id: startfield
            Layout.fillWidth: true
            placeholder: qsTr("The start position of the region.")
            onTextChanged: model["start"] = text;
        }

        Text
        {
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            text: qsTr("End*")
            color: Regovar.theme.primaryColor.back.dark
            font.pixelSize: Regovar.theme.font.size.normal
            font.family: Regovar.theme.font.family
            verticalAlignment: Text.AlignVCenter
            height: 35
            font.bold: true
        }
        TextField
        {
            id: endfield
            Layout.fillWidth: true
            placeholder: qsTr("The end position of the region.")
            onTextChanged: model["end"] = text;
        }

        Item
        {
            Layout.columnSpan: 2
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }

    // Borders
    Rectangle
    {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: 1

        color: Regovar.theme.boxColor.border
    }
    Rectangle
    {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: 1

        color: Regovar.theme.boxColor.border
    }
    Rectangle
    {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: 1

        color: Regovar.theme.boxColor.border
    }
}
