import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

import "../../Regovar"
import "../../Framework"


GenericScreen
{
    id: root
    property real labelColWidth: 100
    readyForNext: true

    onZChanged: checkReady()
    Component.onCompleted: checkReady()

    Text
    {
        id: header
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        text: qsTr("You're almost done! Choose a name and select the project you want to save your analysis to. Below is a summary of the configuration of the analysis.\nIf all is good. press the \"Launch\" button. The Regovar server will prepare your data, and you will then be able to dynamically filter the variants.")
        wrapMode: Text.WordWrap
        font.pixelSize: Regovar.theme.font.size.control
        color: Regovar.theme.primaryColor.back.normal
    }

    RowLayout
    {
        id: projectForm
        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: 30
        spacing: 10

        Text
        {
            height: Regovar.theme.font.size.header
            Layout.minimumWidth: root.labelColWidth
            text: qsTr("Project")
            color: Regovar.theme.frontColor.normal
            font.pixelSize: Regovar.theme.font.size.control
            font.family: Regovar.theme.font.familly
            verticalAlignment: Text.AlignVCenter
            Component.onCompleted: root.labelColWidth = Math.max(root.labelColWidth, width)
        }

        ComboBox
        {
            Layout.fillWidth: true
            id: projectField
            model: ["Select a project", "DPNI", "Panel onchogénétique", "Hugodims"]
            onCurrentIndexChanged: checkReady();
        }
    }
    RowLayout
    {
        id: nameForm
        anchors.top: projectForm.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: 10
        spacing: 10

        Text
        {
            height: Regovar.theme.font.size.header
            Layout.minimumWidth: root.labelColWidth
            text: qsTr("Name")
            color: Regovar.theme.frontColor.normal
            font.pixelSize: Regovar.theme.font.size.control
            font.family: Regovar.theme.font.familly
            verticalAlignment: Text.AlignVCenter
            Component.onCompleted: root.labelColWidth = Math.max(root.labelColWidth, width)
            onTextChanged: checkReady();
        }
        TextField
        {
            id: nameField
            Layout.fillWidth: true
            placeholderText: qsTr("The name of the analysis")
            text: autoName()
        }
        Button
        {
            text: qsTr("Name auto")
            onClicked: nameField.text = autoName();
        }
    }
    Text
    {
        anchors.top: nameForm.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: 10
        text: qsTr("Analysis summary")
        wrapMode: Text.WordWrap
        font.pixelSize: Regovar.theme.font.size.control
        //color: Regovar.theme.primaryColor.back.normal
    }


    Rectangle
    {
        anchors.top: nameForm.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        anchors.topMargin: Regovar.theme.font.size.control + 15


        color: Regovar.theme.boxColor.back
        border.color: Regovar.theme.boxColor.border
        border.width: 1


        ScrollView
        {
            id: scrollArea
            anchors.fill: parent

            Column
            {
                x: 5
                y: 5
                spacing: 5

                RowLayout
                {
                    width: scrollArea.viewport.width - 10
                    spacing: 10

                    Text
                    {
                        height: Regovar.theme.font.size.header
                        Layout.minimumWidth: root.labelColWidth
                        text: qsTr("Type")
                        color: Regovar.theme.frontColor.disable
                        font.pixelSize: Regovar.theme.font.size.control
                        font.family: Regovar.theme.font.familly
                        font.bold: true
                        verticalAlignment: Text.AlignVCenter
                        Component.onCompleted: root.labelColWidth = Math.max(root.labelColWidth, width)
                    }
                    Text
                    {
                        Layout.fillWidth: true
                        height: Regovar.theme.font.size.header
                        text: "Filtering Trio"
                        color: Regovar.theme.frontColor.disable
                        font.pixelSize: Regovar.theme.font.size.control
                        font.family: Regovar.theme.font.familly
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                RowLayout
                {
                    width: scrollArea.viewport.width - 10
                    spacing: 10

                    Text
                    {
                        height: Regovar.theme.font.size.header
                        Layout.minimumWidth: root.labelColWidth
                        text: qsTr("Referencial")
                        color: Regovar.theme.frontColor.disable
                        font.pixelSize: Regovar.theme.font.size.control
                        font.family: Regovar.theme.font.familly
                        font.bold: true
                        verticalAlignment: Text.AlignVCenter
                        Component.onCompleted: root.labelColWidth = Math.max(root.labelColWidth, width)
                    }
                    Text
                    {
                        Layout.fillWidth: true
                        height: Regovar.theme.font.size.header
                        text: "Hg19"
                        color: Regovar.theme.frontColor.disable
                        font.pixelSize: Regovar.theme.font.size.control
                        font.family: Regovar.theme.font.familly
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                RowLayout
                {
                    width: scrollArea.viewport.width - 10
                    spacing: 10

                    Text
                    {
                        Layout.alignment: Qt.AlignTop
                        height: Regovar.theme.font.size.header
                        Layout.minimumWidth: root.labelColWidth
                        text: qsTr("Samples")
                        color: Regovar.theme.frontColor.disable
                        font.pixelSize: Regovar.theme.font.size.control
                        font.family: Regovar.theme.font.familly
                        font.bold: true
                        verticalAlignment: Text.AlignVCenter
                        Component.onCompleted: root.labelColWidth = Math.max(root.labelColWidth, width)
                    }
                    Column
                    {
                        Layout.fillWidth: true
                        Row
                        {

                            Text
                            {
                                width: Regovar.theme.font.boxSize.control
                                font.pixelSize: Regovar.theme.font.size.control
                                font.family: Regovar.theme.icons.name
                                color: Regovar.theme.frontColor.disable
                                verticalAlignment: Text.AlignVCenter
                                text: "9"
                            }

                            Text
                            {
                                font.pixelSize: Regovar.theme.font.size.control
                                color: Regovar.theme.frontColor.disable
                                verticalAlignment: Text.AlignVCenter
                                font.family: "monospace"
                                text: "MD-02-75"+ " - "
                            }
                            Text
                            {
                                font.pixelSize: Regovar.theme.font.size.control
                                color: Regovar.theme.frontColor.disable
                                verticalAlignment: Text.AlignVCenter
                                text: "DUPONT Michel (64y)"
                            }
                            Text
                            {
                                width: Regovar.theme.font.boxSize.control
                                font.pixelSize: Regovar.theme.font.size.control
                                font.family: Regovar.theme.icons.name
                                color: Regovar.theme.frontColor.disable
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                text: "{"
                            }
                            Text
                            {
                                font.pixelSize: Regovar.theme.font.size.control
                                color: Regovar.theme.frontColor.disable
                                verticalAlignment: Text.AlignVCenter
                                font.family: "monospace"
                                text: "Hp-A7-5663"
                            }
                        }
                        Row
                        {

                            Text
                            {
                                width: Regovar.theme.font.boxSize.control
                                font.pixelSize: Regovar.theme.font.size.control
                                font.family: Regovar.theme.icons.name
                                color: Regovar.theme.frontColor.disable
                                verticalAlignment: Text.AlignVCenter
                                text: "<"
                            }

                            Text
                            {
                                font.pixelSize: Regovar.theme.font.size.control
                                color: Regovar.theme.frontColor.disable
                                verticalAlignment: Text.AlignVCenter
                                font.family: "monospace"
                                text: "MD-02-76"+ " - "
                            }
                            Text
                            {
                                font.pixelSize: Regovar.theme.font.size.control
                                color: Regovar.theme.frontColor.disable
                                verticalAlignment: Text.AlignVCenter
                                text: "DUPONT Micheline (60y)"
                            }
                            Text
                            {
                                width: Regovar.theme.font.boxSize.control
                                font.pixelSize: Regovar.theme.font.size.control
                                font.family: Regovar.theme.icons.name
                                color: Regovar.theme.frontColor.disable
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                text: "{"
                            }
                            Text
                            {
                                font.pixelSize: Regovar.theme.font.size.control
                                color: Regovar.theme.frontColor.disable
                                verticalAlignment: Text.AlignVCenter
                                font.family: "monospace"
                                text: "Gtk-8999112"
                            }
                        }
                        Row
                        {

                            Text
                            {
                                width: Regovar.theme.font.boxSize.control
                                font.pixelSize: Regovar.theme.font.size.control
                                font.family: Regovar.theme.icons.name
                                color: Regovar.theme.frontColor.disable
                                verticalAlignment: Text.AlignVCenter
                                text: "9"
                            }

                            Text
                            {
                                font.pixelSize: Regovar.theme.font.size.control
                                color: Regovar.theme.frontColor.disable
                                verticalAlignment: Text.AlignVCenter
                                font.family: "monospace"
                                text: "MM-11-86"+ " - "
                            }
                            Text
                            {
                                font.pixelSize: Regovar.theme.font.size.control
                                color: Regovar.theme.frontColor.disable
                                verticalAlignment: Text.AlignVCenter
                                text: "DUPONT Michou (11y)"
                            }
                            Text
                            {
                                width: Regovar.theme.font.boxSize.control
                                font.pixelSize: Regovar.theme.font.size.control
                                font.family: Regovar.theme.icons.name
                                color: Regovar.theme.frontColor.disable
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                text: "{"
                            }
                            Text
                            {
                                font.pixelSize: Regovar.theme.font.size.control
                                color: Regovar.theme.frontColor.disable
                                verticalAlignment: Text.AlignVCenter
                                font.family: "monospace"
                                text: "GHJ-446"
                            }
                        }
                    }
                }
                RowLayout
                {
                    width: scrollArea.viewport.width - 10
                    spacing: 10

                    Text
                    {
                        Layout.alignment: Qt.AlignTop
                        height: Regovar.theme.font.size.header
                        Layout.minimumWidth: root.labelColWidth
                        text: qsTr("Annotations")
                        color: Regovar.theme.frontColor.disable
                        font.pixelSize: Regovar.theme.font.size.control
                        font.family: Regovar.theme.font.familly
                        font.bold: true
                        verticalAlignment: Text.AlignVCenter
                        Component.onCompleted: root.labelColWidth = Math.max(root.labelColWidth, width)
                    }
                    Column
                    {
                        Layout.fillWidth: true
                        Text
                        {
                            font.pixelSize: Regovar.theme.font.size.control
                            color: Regovar.theme.frontColor.disable
                            verticalAlignment: Text.AlignVCenter
                            text: "VEP (v76)"
                        }
                        Text
                        {
                            font.pixelSize: Regovar.theme.font.size.control
                            color: Regovar.theme.frontColor.disable
                            verticalAlignment: Text.AlignVCenter
                            text: "SnpEff (v4.2)"
                        }
                        Text
                        {
                            font.pixelSize: Regovar.theme.font.size.control
                            color: Regovar.theme.frontColor.disable
                            verticalAlignment: Text.AlignVCenter
                            text: "dbNSFP (v3.3)"
                        }
                    }
                }
            }
        }
    }



    function checkReady()
    {
        readyForNext = true; //refField.currentIndex > 0;
    }

    function autoName()
    {
        var name = qsTr("Trio");
        name += "-MD-02-75-2017-09.15";
        name += Qt.formatDate(Date.now(), "yyyy-MM-dd");
        return name;
    }

}
