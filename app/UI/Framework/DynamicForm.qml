import QtQuick 2.7
import "../Regovar"

Column
{
    id: root
    property var model

    Repeater
    {
        model: root.model

        DynamicFormInput
        {
            width: root.width
            height: 25
            model: modelData
        }
    }
}