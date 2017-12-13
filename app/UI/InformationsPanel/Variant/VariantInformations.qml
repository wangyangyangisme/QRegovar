import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import org.regovar 1.0

import "../../Regovar"
import "../../Framework"


Rectangle
{
    id: root
    color: Regovar.theme.backgroundColor.main

    property var model
    onModelChanged:  if (model) { updateFromModel(model); }


    property string varId
    onVarIdChanged:
    {
        if (varId)
        {
            // Display loading feedback

            // request informations

        }
        else
        {
            // Display help message
        }
    }

    function updateFromModel(data)
    {
        var variant = "chr" + data["chr"] + ":" + data["pos"] + " " + data["ref"] + ">" + data["alt"];
        var gene = data["genename"];
        var ref = data["reference"];
        title.text = "<h1 style=\"font-family: monospace;\">" + variant + "</h1><br/><br/>Gene: <i>" + gene + "</i><br/>Ref: <i>" + ref + "</i>";
    }





    ColumnLayout
    {
        anchors.fill: parent
        spacing: 0

        Rectangle
        {
            id: header
            Layout.fillWidth: true
            Layout.minimumHeight: 100 // icon : 80 + 2*10
            color: Regovar.theme.primaryColor.back.normal

            Text
            {
                id: icon
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.margins: 10
                width: 80
                height: 80
                font.pixelSize: 80
                font.family: Regovar.theme.icons.name
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                color: Regovar.theme.primaryColor.front.normal
                text: "ì"
            }
            TextEdit
            {
                id: title
                anchors.top: parent.top
                anchors.left: icon.right
                anchors.right: parent.right
                anchors.margins: 10
                textFormat: TextEdit.RichText
                onPaintedHeightChanged: { header.Layout.minimumHeight = Math.max(100, paintedHeight + 20); }
                font.pixelSize: Regovar.theme.font.size.normal
                color: Regovar.theme.primaryColor.front.normal
                readOnly: true
                selectByMouse: true
                selectByKeyboard: true
                wrapMode: TextEdit.Wrap
            }
        }

        Rectangle
        {
            Layout.fillHeight: true
            Layout.fillWidth: true
            TabView
            {
                id: swipeview
                anchors.fill : parent
                tabSharedModel: root.model
                smallHeader: true


                tabsModel: ListModel
                {
                    ListElement
                    {
                        title: qsTr("Variant")
                        icon: "j"
                        source: "../InformationsPanel/Variant/InfoPanel.qml"
                    }
//                    ListElement
//                    {
//                        title: qsTr("Gene")
//                        icon: "j"
//                        source: "../InformationsPanel/Gene/InfoPanel.qml"
//                    }
//                    ListElement
//                    {
//                        title: qsTr("Phenotype")
//                        icon: "K"
//                        source: "../InformationsPanel/Phenotype/InfoPanel.qml"
//                    }
                    ListElement
                    {
                        title: qsTr("Regovar statistics")
                        icon: "^"
                        source: "../InformationsPanel/Variant/StatsPanel.qml"
                    }
                }
            }
        }
    }

}
