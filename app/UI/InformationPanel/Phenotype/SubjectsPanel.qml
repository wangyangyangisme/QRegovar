import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import Regovar.Core 1.0

import "qrc:/qml/Regovar"
import "qrc:/qml/Framework"
import "qrc:/qml/InformationPanel/Common"

Item
{
    id: root
    property var model
    onModelChanged: updateFromModel(model)
    Component.onCompleted: updateFromModel(model)

    ColumnLayout
    {
        id: content
        anchors.fill: parent
        anchors.margins: 10
        visible: false
        enabled: false

        TextField
        {
            id: searchField
            Layout.fillWidth: true
            iconLeft: "z"
            displayClearButton: true
            placeholder: qsTr("Quick filter...")
            onTextEdited: subjectsTable.model.setFilterString(text)
        }

        TableView
        {
            id: subjectsTable
            Layout.fillWidth: true
            Layout.fillHeight: true

            TableViewColumn
            {
                role: "identifier"
                title: qsTr("Identifier")
                width: 75
                delegate: RowLayout
                {
                    anchors.left: parent.left
                    anchors.leftMargin: 5
                    spacing: 10

                    ButtonInline
                    {
                        iconTxt: "z"
                        text: ""
                        onClicked: regovar.subjectsManager.openSubject(model.id)
                    }
                    Text
                    {
                        Layout.fillWidth: true
                        font.pixelSize: Regovar.theme.font.size.normal
                        font.family: Regovar.theme.font.family
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                        text: styleData.value
                    }
                }
            }
            TableViewColumn
            {
                role: "lastname"
                title: qsTr("Lastname")
            }
            TableViewColumn
            {
                role: "firstname"
                title: qsTr("Firstname")
            }
            TableViewColumn
            {
                role: "sex"
                title: qsTr("Sex")
                width: 75
            }
            TableViewColumn
            {
                role: "dateOfBirth"
                title: qsTr("Date of birth")
                width: 100
            }
            TableViewColumn
            {
                role: "comment"
                title: qsTr("Comment")
            }
        }

        Text
        {
            id: label
            Layout.fillWidth: true
            font.pixelSize: Regovar.theme.font.size.small
            horizontalAlignment: Text.AlignRight
        }
    }

    Text
    {
        id: emptyMessage
        anchors.fill: parent
        wrapMode: Text.WordWrap
        elide: Text.ElideRight
        text: qsTr("No subject")
        color: Regovar.theme.primaryColor.back.normal
        font.pixelSize: Regovar.theme.font.size.header
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    function updateFromModel(data)
    {
        if (!data) return;
        var count = data.subjects.rowCount();

        if (count>0)
        {
            content.visible = true;
            content.enabled = true;
            emptyMessage.visible = false;
            subjectsTable.model = data.subjects.proxy;
            label.text = count + " subjects.";
        }
        else
        {
            content.visible = false;
            content.enabled = false;
            emptyMessage.visible = true;
            subjectsTable.model = null;
        }
    }
}
