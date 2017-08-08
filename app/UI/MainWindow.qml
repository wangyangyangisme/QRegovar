import QtQuick 2.7
import Qt.labs.settings 1.0

import "Regovar"

GenericWindow
{
    id: root
    width: 800
    height: 600

    menuModel: Regovar.menuModel
    title: "Regovar - " + menuModel.mainTitle

    Settings
    {
        property alias x: root.x
        property alias y: root.y
        property alias width: root.width
        property alias height: root.height
    }
}
