import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4

import "qrc:/qml/Regovar"
import "qrc:/qml/Framework"
import "qrc:/qml/Dialogs"


GenericScreen
{
    id: root

    readyForNext: true


    // Help information on this page
    Box
    {
        id: helpInfoBox
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        visible: Regovar.helpInfoBoxDisplayed
        icon: "k"
        text: qsTr("Select the file(s) you want to analyse. You can select files that are already available on the server, or upload your own files.")
    }

    RowLayout
    {
        anchors.top: Regovar.helpInfoBoxDisplayed ? helpInfoBox.bottom : parent.top
        anchors.topMargin: Regovar.helpInfoBoxDisplayed ? 10 : 0
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        spacing: 10


        TableView
        {
            id: inputsList
            clip: true
            Layout.fillWidth: true
            Layout.fillHeight: true

            model: regovar.analysesManager.newPipeline.inputsFiles


            Rectangle
            {
                id: dropAreaFeedBack
                anchors.fill: parent;
                color: "#99ffffff"
                visible: false
                Text
                {
                    anchors.centerIn: parent
                    text: qsTr("Drop your files here")
                }
            }

            DropArea
            {
                id: dropArea;
                anchors.fill: parent;
                onEntered:
                {
                    if (drag.hasUrls)
                    {
                        dropAreaFeedBack.visible = true;
                        drag.accept (Qt.CopyAction);
                    }
                }
                onDropped:
                {
                    var files= []
                    for(var i=0; i<drop.urls.length; i++)
                    {
                        files = files.concat(drop.urls[i]);
                    }
                    regovar.filesManager.enqueueUploadFile(files);
                    dropAreaFeedBack.visible = false;
                }
                onExited: dropAreaFeedBack.visible = false;
            }

            TableViewColumn
            {
                title: "Name"
                role: "name"
                delegate: Item
                {

                    Text
                    {
                        anchors.leftMargin: 5
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: styleData.textAlignment
                        font.pixelSize: Regovar.theme.font.size.normal
                        text: styleData.value.icon
                        font.family: Regovar.theme.icons.name
                    }
                    Text
                    {
                        anchors.leftMargin: Regovar.theme.font.boxSize.normal + 5
                        anchors.rightMargin: 5
                        anchors.left: parent.left
                        anchors.right: parent.right
                        verticalAlignment: Text.AlignVCenter
                        anchors.fill: parent
                        horizontalAlignment: styleData.textAlignment
                        font.pixelSize: Regovar.theme.font.size.normal
                        text: styleData.value.filename
                        elide: Text.ElideRight
                    }
                }
            }
            TableViewColumn
            {
                title: "Status"
                role: "status"
                delegate: Item
                {

                    Text
                    {
                        anchors.leftMargin: 5
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: styleData.textAlignment
                        font.pixelSize: Regovar.theme.font.size.normal
                        text: styleData.value.status === "uploading" ? "/" : styleData.value.status === "error" ? "l" : "n"
                        font.family: Regovar.theme.icons.name
                        onTextChanged:
                        {
                            if (text == "/")
                            {
                                iconAnimation.start();
                            }
                            else
                            {
                                iconAnimation.stop();
                                rotation = 0;
                            }
                        }

                        NumberAnimation on rotation
                        {
                            id: iconAnimation
                            duration: 1500
                            loops: Animation.Infinite
                            from: 0
                            to: 360
                        }
                    }
                    Text
                    {
                        anchors.leftMargin: Regovar.theme.font.boxSize.normal + 5
                        anchors.rightMargin: 5
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        horizontalAlignment: styleData.textAlignment
                        font.pixelSize: Regovar.theme.font.size.normal
                        text: styleData.value.label
                        elide: Text.ElideRight
                    }

                }
            }
            TableViewColumn { title: "Size"; role: "size"; horizontalAlignment: Text.AlignRight }
            TableViewColumn { title: "Date";  role: "updateDate" }
            TableViewColumn { title: "Source"; role: "source" }
            TableViewColumn { title: "Comment"; role: "comment" }
        }




        Column
        {
            Layout.alignment: Qt.AlignTop
            spacing: 10
            Button
            {
                id: addButton
                text: qsTr("Add file")
                onClicked: { fileSelector.reset(); fileSelector.open(); }
            }
            Button
            {
                id: remButton
                text: qsTr("Remove file")
                onClicked:
                {
                    // Get list of objects to remove
                    var files= []
                    inputsList.selection.forEach( function(rowIndex)
                    {
                        files = files.concat(regovar.filesManager.remoteList[rowIndex]);
                    });
                    regovar.analysesManager.newPipeline.removeInputs(files);
                }
            }
        }
    }



    SelectFilesDialog
    {
        id: fileSelector
        onFileSelected: regovar.analysesManager.newPipeline.addInputs(files)
    }
}
