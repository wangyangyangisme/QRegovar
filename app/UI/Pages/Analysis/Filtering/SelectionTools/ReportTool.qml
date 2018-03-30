import QtQuick 2.9
import QtGraphicalEffects 1.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import Regovar.Core 1.0

import "qrc:/qml/Regovar"
import "qrc:/qml/Framework"
import "qrc:/qml/Pages/Analysis/Filtering/Quickfilter"


QuickFilterBox
{
    id: root
    title : qsTr("Report")
    isExpanded: false

    property bool internalUiUpdate: false
    property var parameters
    property int currentIndex: 0



    content: Column
    {
        id: content
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right



        Rectangle { width: root.width; height: 5; color: "transparent" }
        GridLayout
        {
            width: content.width - 50
            anchors.left: parent.left
            anchors.leftMargin: 30

            columnSpacing: 10
            rowSpacing: 5
            columns: 2
            rows: 2

            Text
            {

                elide: Text.ElideRight
                font.pixelSize: Regovar.theme.font.size.normal
                color: Regovar.theme.primaryColor.back.dark
                text: qsTr("Report:")
            }

            ComboBox
            {
                id: reportsCombo
                Layout.fillWidth: true
                model: regovar.toolsManager.reporters
                textRole: "name"
                onCurrentIndexChanged:
                {
                    root.parameters = regovar.toolsManager.reporters[currentIndex].parameters;
                    description.text = regovar.toolsManager.reporters[currentIndex].description;
                    root.currentIndex = currentIndex;
                }
            }

            Text
            {
                Layout.row: 1
                Layout.column: 1
                Layout.fillWidth: true
                text: qsTr("Select the report you want to generate.")
                font.pixelSize: Regovar.theme.font.size.small
                font.italic: true
                color: Regovar.theme.primaryColor.back.normal
                wrapMode: Text.WordWrap
            }
        }

        Rectangle
        {
            visible: root.parameters && root.parameters.length > 0
            width: root.width
            height: 20
            color: "transparent"
            Rectangle
            {
                width: content.width - 50
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 30
                height: 1
                color: Regovar.theme.primaryColor.back.normal
            }
        }

        Text
        {
            id: description
            width: content.width - 50
            anchors.left: parent.left
            anchors.leftMargin: 30
            visible: root.parameters && root.parameters.length > 0
            height: 20
            font.pixelSize: Regovar.theme.font.size.small
            font.italic: true
            color: Regovar.theme.primaryColor.back.normal
            wrapMode: Text.WordWrap
        }

        Rectangle
        {
            visible: root.parameters && root.parameters.length > 0
            width: root.width
            height: 20
            color: "transparent"
        }

        DynamicForm
        {
            width: content.width - 50
            anchors.left: parent.left
            anchors.leftMargin: 30

            visible: root.parameters && root.parameters.length > 0
            model: root.parameters
        }

        Rectangle { width: root.width; height: 5; color: "transparent" }
    }
}
