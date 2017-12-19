import QtQuick 2.9
import QtQuick.Layouts 1.3

import "../../Regovar"

Item
{
    id: root
    width: 250
    height: 250

    property var model
    onModelChanged:
    {
        if (model)
        {
            updateViewFromModel(model);
        }
    }

    //
    // Header
    //
    Text
    {
        anchors.top: root.top
        anchors.left: root.left
        text: qsTr("Overview")
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

        property double minLabelWidth: 30
        ColumnLayout
        {
            anchors.fill: parent
            anchors.margins: 5

            Repeater
            {
                id: repeater

                RowLayout
                {
                    width: parent.width
                    spacing: 5

                    Text
                    {
                        Layout.minimumWidth: content.minLabelWidth
                        font.pixelSize: Regovar.theme.font.size.normal
                        color: Regovar.theme.primaryColor.back.normal
                        height: Regovar.theme.font.boxSize.normal
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                        text: modelData.label
                        onPaintedWidthChanged: content.minLabelWidth = Math.max(content.minLabelWidth, paintedWidth)
                    }
                    Text
                    {
                        Layout.fillWidth: true
                        font.pixelSize: Regovar.theme.font.size.normal
                        color: Regovar.theme.primaryColor.back.normal
                        height: Regovar.theme.font.boxSize.normal
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignRight
                        elide: Text.ElideRight
                        text: modelData.value
                    }
                }
            }
            Item
            {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
        }
    }


    function updateViewFromModel(sample)
    {
        var data=[];
        if (sample)
        {
            data.push({"label": qsTr("VCF file"), "value": sample.source.name});
            data.push({"label": qsTr("VCF import"), "value": Regovar.formatDate(sample.source.creationDate)});
            data.push({"label": qsTr("BAM file"), "value": "-"});
            data.push({"label": qsTr("VCF header reference"), "value": sample.reference.name}); // TODO: this stat must be computed by import manager server side
            data.push({"label": qsTr("Total variants"), "value": Regovar.formatBigNumber(sample.stats["total_variant"])});
            data.push({"label": qsTr("Total transcript"), "value": Regovar.formatBigNumber(sample.stats["total_transcript"])});
            data.push({"label": qsTr("Overlapped genes"), "value": "-"});

        }
        repeater.model = data;
    }

}
