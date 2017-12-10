import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import org.regovar 1.0

import "../../Regovar"
import "../../Framework"
import "../Common"


ScrollView
{
    id: root
    anchors.fill: parent
    horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff

    property var model
    onModelChanged:  if (model) { updateFromModel(model); }

    function updateFromModel(data)
    {
        if (data)
        {
            if ("gene" in data)
                geneData = data["gene"];
            else
                geneData = data;
        }
    }

    property var geneData


    Column
    {
        x: 10
        y: 10

        TextEdit
        {
            width: root.width - 30
            text: formatInfo(geneData)
            textFormat: TextEdit.RichText
            font.pixelSize: Regovar.theme.font.size.normal + 2
            color: Regovar.theme.frontColor.normal
            readOnly: true
            selectByMouse: true
            selectByKeyboard: true
            wrapMode: TextEdit.Wrap
            onLinkActivated: Qt.openUrlExternally(link)
        }
        Item
        {
            width: 1
            height: 10
        }
    }

    function formatInfo(data)
    {
        var text = "<table>";
        text += "<tr><td><b>Locus type:</b></td><td>" + data["locus_type"] + "</td></tr>";
        // Former names/symbols
        text += "<tr><td><b>Former names:</b></td><td>";
        if (data["prev_name"].length > 1)
        {
            text += "<ul>";
            for (var idx=0; idx<data["prev_name"].length; idx++)
            {
                text += "<li>" ;
                if ("prev_symbol" in data)
                {
                    text += data["prev_symbol"][idx] + ": ";
                }
                text += data["prev_name"][idx] + "</li>";
            }
        }
        else if (data["prev_name"].length == 1)
        {
            text += "<li>";
            if ("prev_symbol" in data)
            {
                text += data["prev_symbol"][0] + ": ";
            }
            text += data["prev_name"][0] + "</li>";
        }
        text += "</td></tr>";

        // Synonyms
        text += "<tr><td><b>Synonyms:</b></td><td>";
        for (var idx=0; idx<data["alias_symbol"].length; idx++)
        {
            text += data["alias_symbol"][idx] + ", ";
        }
        text += "</td></tr>";

        // Genomes references
        if ("refgene" in data && data["refgene"].length>0)
        {
            text += "<tr><td><b>Genome reference:</b></td><td>";
            if (data["refgene"].length > 1)
            {
                text += "<ul>";
                for (var idx=0; idx<data["refgene"].length; idx++)
                {
                    text += "<li>" + data["refgene"][idx]["name"] + ": " + formatSize(data["refgene"][idx]) + "</li>";
                }
                text += "</ul>";
            }
            else
            {
                text += data["refgene"][0]["name"] + ": " + formatSize(data["refgene"][0]);
            }

            text += "</td></tr>";
        }
        // text += "<tr><td><b>:</b></td><td>" + data[""] + "</td></tr>";




        text += "</table><br><br>";

        if (data["omim_description"])
        {
            text += "<b>Omim description:</b><br>" + data["omim_description"];
        }

        return text;
    }

    function formatSize(data)
    {
        var size = Math.round(data["size"]/1000, 0);
        size = (size > 0) ? size + " Kb" : data["size"] + " b";
        var exons = data["exon"];
        exons += " " + ((exons > 1) ? qsTr("exons") : qsTr("exon"));
        var trx = data["trx"];
        trx += " " + ((trx > 1) ? qsTr("transcripts") : qsTr("transcript"));
        return size + ", " + exons + ", " + trx;
    }
}
