import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Regovar.Core 1.0

import "../../Regovar"
import "../../Framework"
import "../Common"


InformationPanel
{
    id: root
    icon: "j"
    updateFromModel: function updateFromModel(data)
    {
        // Update title
        var variant = "chr" + data["chr"] + ":" + data["pos"] + " " + data["ref"] + ">" + data["alt"];
        var gene = data["genename"];
        var ref = data["reference"];
        root.title = "<span style=\"font-family: monospace;\">" + variant + "</span><br/><br/><i>Ref: </i>" + ref + "&nbsp;&nbsp;&nbsp;</span>\n\n<i>Gene: </i>" + gene;

        // Update tabs
        root.tabSharedModel = data;
        var ttt = listModel.createObject(root);
        ttt.append(
            {   "title": qsTr("Information"),
                "icon": "è",
                "source": "qrc:/qml/InformationPanel/Phenotype/InfoPanel.qmll"
            });
        ttt.append({
                "title": qsTr("Online tools"),
                "icon": "K",
                "source": "qrc:/qml/InformationPanel/Phenotype/OnlineToolsPanel.qml"
            });
        ttt.append({
                "title": qsTr("Regovar statistics"),
                "icon": "í",
                "source": "qrc:/qml/InformationPanel/Phenotype/StatsPanel.qml"
            });
        root.tabsModel = ttt;
        root.loading = false;
    }

    Component
    {
        id:listModel
        ListModel {}
    }

    Connections
    {
        target: regovar
        onPhenotypeInformationReady: root.model = json
    }
}
