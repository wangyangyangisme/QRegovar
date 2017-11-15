import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import org.regovar 1.0
import QtQml.Models 2.2

import "../../../Regovar"
import "../../../Framework"
import "../../../MainMenu"

import "Quickfilter"
import org.regovar 1.0

Rectangle
{
    id: root
    color: Regovar.theme.backgroundColor.main

    property FilteringAnalysis model
    onModelChanged:
    {
        if (root.model != null)
        {
            resultsTree.model = root.model.results;
            resultsTree.rowHeight = (root.model.samples.length === 1) ? 25 : root.model.samples.length * 18;
        }
    }

    Rectangle
    {
        id: header
        anchors.left: rightPanel.left
        anchors.top: rightPanel.top
        anchors.right: rightPanel.right
        height: 50
        color: Regovar.theme.backgroundColor.alt


        RowLayout
        {
            anchors.bottom: header.bottom
            anchors.top: header.top
            anchors.left: header.left
            anchors.margins: 10
            spacing: 5

            SaveFilterButton
            {
                Layout.fillWidth: true
//                text: qsTr("Current filter")
//                iconText: "5"
                text: model.currentFilterName ? model.currentFilterName : qsTr("Save new filter")
                enabled: model.currentFilterName == ""
                iconText: model.currentFilterName ? "D" : "5"
                height: header.height - 20
                onClicked:
                {
                    model.emitDisplayFilterSavingFormPopup();
                }
            }

            Text
            {
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignRight
                color: Regovar.theme.primaryColor.back.dark

                font.pixelSize: Regovar.theme.font.size.header
                text: ( root.model != null) ? Regovar.formatBigNumber(root.model.results.total) + " " + ((root.model.results.total > 1) ? qsTr("variants") : qsTr("variant")) : ""
            }
        }
        ConnectionStatus
        {
            anchors.top: header.top
            anchors.right: header.right
            anchors.bottom: header.bottom
            anchors.margins: 5
            anchors.rightMargin: 10
        }

    }

    // Help information on this page
    Box
    {
        id: helpInfoBox
        anchors.top : header.bottom
        anchors.left: root.left
        anchors.right: root.right
        anchors.margins: 10
        height: 30

        visible: Regovar.helpInfoBoxDisplayed
        mainColor: Regovar.theme.frontColor.success
        icon: "k"
        text: qsTr("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aliquam tristique eu lorem sit amet viverra. Vivamus vitae fringilla nibh. Mauris tempor neque eu mauris tristique consequat. Curabitur dui enim, tempor ut quam vel, sagittis tempus diam. Praesent eu erat ante.")
    }


    RowLayout
    {
        id: controlPanel
        anchors.top : header.bottom
        anchors.left: root.left
        anchors.right: root.right
        anchors.margins: 10
        anchors.topMargin: Regovar.helpInfoBoxDisplayed ? helpInfoBox.height + 20 : 10

        spacing: 10

        TextField
        {
            Layout.fillWidth: true
            iconLeft: "z"
            placeholderText: qsTr("Search by position chr1:422566 or by genes names...")
        }
        ComboBox
        {
            model: ["Group By", "Gene Name"]
        }

    }



    VariantsTable
    {
        id: resultsTree
        anchors.top: controlPanel.bottom
        anchors.left: root.left
        anchors.right: root.right
        anchors.bottom: root.bottom
        anchors.margins: 10
    }



    Rectangle
    {
        id: busyIndicator
        anchors.fill: rightPanel
        anchors.margins: 10
        anchors.topMargin: 60

        color: "#aaffffff"
        visible: !regovar.newFilteringAnalysis.isLoading

        MouseArea
        {
            anchors.fill: parent
        }

        BusyIndicator
        {
            anchors.centerIn: parent
        }
    }

}
