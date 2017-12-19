import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4
import QtCharts 2.0

import "../../Regovar"

Item
{
    id: root
    width: 450
    height: 250

    property var model
    onModelChanged:
    {
        if (model)
        {
            updateStatistics(model);
        }
    }

    //
    // Header
    //
    Text
    {
        anchors.top: root.top
        anchors.left: root.left
        text: qsTr("Variant classes")
        font.pixelSize: Regovar.theme.font.size.normal
        color: Regovar.theme.primaryColor.back.normal
        height: Regovar.theme.font.boxSize.normal
        verticalAlignment: Text.AlignVCenter
        font.bold: true
    }

    //
    // Content
    //
    Rectangle
    {
        id: content
        anchors.fill: parent
        anchors.topMargin: Regovar.theme.font.boxSize.normal
        color: Regovar.theme.boxColor.back
        border.width: 1
        border.color: Regovar.theme.boxColor.border
        clip: true

        RowLayout
        {
            anchors.fill: parent
            anchors.margins: 5
            spacing: 5

            Column
            {
                id: variantClassesLayout
                Layout.alignment: Qt.AlignTop
                Layout.fillWidth: true

                Item
                {
                    width: variantClassesLayout.width
                    height: Regovar.theme.font.boxSize.normal
                    Text
                    {
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("Total variants")
                        font.pixelSize: Regovar.theme.font.size.normal
                        color: Regovar.theme.primaryColor.back.normal
                        height: Regovar.theme.font.boxSize.normal
                        verticalAlignment: Text.AlignVCenter
                        font.bold: true
                    }
                    Text
                    {
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        id: totalVariant
                        Layout.alignment: Qt.AlignRight
                        text: "-"
                        font.pixelSize: Regovar.theme.font.size.normal
                        color: Regovar.theme.primaryColor.back.normal
                        height: Regovar.theme.font.boxSize.normal
                        verticalAlignment: Text.AlignVCenter
                        font.bold: true
                    }
                }

                Repeater
                {
                    id: variantClassesRepeater

                    Rectangle
                    {
                        width: variantClassesLayout.width
                        height: Regovar.theme.font.boxSize.normal
                        property bool hovered: false
                        color: !hovered ? "transparent" : Regovar.theme.secondaryColor.back.light

                        Text
                        {
                            anchors.left: parent.left
                            anchors.verticalCenter: parent.verticalCenter
                            text: " - " + modelData.label + " (" + modelData.percent + ")"
                            font.pixelSize: Regovar.theme.font.size.normal
                            color: Regovar.theme.primaryColor.back.normal
                            height: Regovar.theme.font.boxSize.normal
                            verticalAlignment: Text.AlignVCenter
                        }
                        Text
                        {
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            text:  modelData.count
                            font.pixelSize: Regovar.theme.font.size.normal
                            color: Regovar.theme.primaryColor.back.normal
                            height: Regovar.theme.font.boxSize.normal
                            verticalAlignment: Text.AlignVCenter
                        }
                        MouseArea
                        {
                            anchors.fill: parent
                            hoverEnabled: true
                            onEntered: { root.highlightPieSection(variantClassesPieSeries, index, true); parent.hovered = true; }
                            onExited: { root.highlightPieSection(variantClassesPieSeries, index, false); parent.hovered = false; }
                        }
                    }
                }

                Item { width: 10; height: 10; }
            }

            ChartView
            {
                height: parent.height
                width: parent.height
                antialiasing: true
                animationOptions: ChartView.AllAnimations
                backgroundColor: content.color
                legend.visible: false
                margins.top: 0
                margins.bottom: 0
                margins.left: 0
                margins.right: 0


                PieSeries
                {
                    id: variantClassesPieSeries
                    property string hoveredSerie: ""
                }
            }
        }
    }


    function updateViewFromModel(model)
    {
        // todo
    }

    function updateStatistics(sample)
    {
        if (sample)
        {
            var stats = sample.stats;
            var variantTotal = stats["sample_total_variant"];

            // Compute : Variant classes
            var variantClasses = ["not", "ref", "snv", "mnv", "insertion", "deletion", "others"];
            var vclassesNames = {"not": qsTr("Not in this sample"), "ref": qsTr("Reference"), "snv": qsTr("SNV"), "mnv": qsTr("MNV"), "insertion": qsTr("Insertion"), "deletion": qsTr("Deletion"), "others": qsTr("Others")};
            var variantClassesChartModel = [];
            variantClasses.forEach(function (vclass)
            {
                var count = stats["variants_classes"][vclass];
                variantClassesChartModel.push({
                    "label": vclassesNames[vclass],
                    "percent": (count / variantTotal * 100.0).toFixed(1) + " %",
                    "count": Regovar.formatBigNumber(count),
                    "value": count / variantTotal * 100.0 });
            });
            // Populate legend
            totalVariant.text = Regovar.formatBigNumber(variantTotal);
            variantClassesRepeater.model = variantClassesChartModel;
            // Populate Pie slices
            variantClassesPieSeries.clear()
            for (var idx=0; idx<variantClassesChartModel.length; idx++)
            {
                variantClassesPieSeries.append(variantClassesChartModel[idx]["percent"], variantClassesChartModel[idx]["value"]);
                var slice = variantClassesPieSeries.at(idx);
                slice.labelVisible = false;
            }
        }
    }


    function highlightPieSection(pieChart, index, exploded)
    {
        pieChart.at(index).exploded = exploded;
    }
}