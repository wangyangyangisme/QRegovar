import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

import "../Framework"
import "../Regovar"
import "../Dialogs"
import "Browse"

Rectangle
{
    id: root
    property QtObject model

    color: Regovar.theme.backgroundColor.main


    Rectangle
    {
        id: header
        anchors.left: root.left
        anchors.top: root.top
        anchors.right: root.right
        height: 50
        color: Regovar.theme.backgroundColor.alt





        ConnectionStatus
        {
            anchors.top: header.top
            anchors.right: header.right
            anchors.bottom: header.bottom
            anchors.margins: 5
            anchors.rightMargin: 10
        }
    }


    Image
    {
        id: logo
        source: "qrc:/regovar.png"
        sourceSize.height: 125
        anchors.top: header.bottom
        anchors.topMargin: 50
        anchors.horizontalCenter: header.horizontalCenter

        LinearGradient
        {
            anchors.fill: parent
            start: Qt.point(0, logo.height / 3)
            end: Qt.point(0, logo.height)
            gradient: Gradient
            {
                GradientStop { position: 0.0; color: Regovar.theme.logo.color1 }
                GradientStop { position: 1.0; color: Regovar.theme.logo.color2  }
            }
            source: logo
        }
    }


    TextField
    {
        id: searchBar
        anchors.top: logo.bottom
        anchors.horizontalCenter: root.horizontalCenter
        anchors.margins: 10
        width: 600
        anchors.topMargin: 50


        Component.onCompleted: text = regovar.searchRequest

        onEditingFinished:
        {
            if (text != "" && root.visible)
            {
                regovar.search(text);
                Regovar.menuModel.selectedIndex=[1,-1,-1];
            }
        }

        placeholderText: qsTr("Search anything, project, sample, phenotype, analysis, variant, report...")
        focus: true
    }


    Rectangle
    {
        id: panel
        color: "transparent"

        anchors.top: searchBar.bottom
        anchors.bottom: root.bottom
        anchors.left: root.left
        anchors.right: root.right
        anchors.margins: 100
        anchors.topMargin: 10
        anchors.bottomMargin: 10


        RowLayout
        {
            id: newButtonsRow
            anchors.horizontalCenter: panel.horizontalCenter
            anchors.top: panel.top
            height: 100
            spacing: 30

            ButtonWelcom
            {
                Layout.alignment: Qt.AlignHCenter
                text: qsTr("New project")
                onClicked: { newProjectDialog.reset(); newProjectDialog.open(); }
                enabled: regovar.connectionStatus == 0
            }
            ButtonWelcom
            {
                Layout.alignment: Qt.AlignHCenter
                text: qsTr("New analysis")
                onClicked: newAnalysisDialog.open()
                enabled: regovar.connectionStatus == 0
            }
            ButtonWelcom
            {
                Layout.alignment: Qt.AlignHCenter
                text: qsTr("New subject")
                onClicked: newSubjectDialog.open()
                enabled: regovar.connectionStatus == 0
            }
        }

        ScrollView
        {
            id: scrollViewArea
            visible: regovar.welcomIsLoading
            anchors.top: newButtonsRow.bottom
            anchors.topMargin: 70
            anchors.left: panel.left
            anchors.right: panel.right
            anchors.bottom: panel.bottom

            horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff

            Column
            {
                spacing: 30


                Rectangle
                {
                    color: "transparent"
                    // Layout.minimumHeight: 3*Regovar.theme.font.boxSize.control
                    width: scrollViewArea.width

                    SplitView
                    {
                        id: row
                        width: parent.width
                        onHeightChanged: parent.height = height




                        Rectangle
                        {
                            id: analysesScrollArea
                            color: "transparent"
                            width: 500
                            // height is sized by colomn content
                            onHeightChanged: row.height = Math.max(height, row.height)
                            clip: true

                            Text
                            {
                                id: analysesHeader
                                verticalAlignment: Text.AlignVCenter
                                elide: Text.ElideRight
                                font.pixelSize: Regovar.theme.font.size.header
                                color: Regovar.theme.primaryColor.back.dark
                                height: Regovar.theme.font.boxSize.header
                                text: qsTr("Last analyses")
                            }

                            Rectangle
                            {
                                anchors.top: analysesHeader.bottom
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.rightMargin: 15
                                height: 1
                                color: Regovar.theme.primaryColor.back.normal
                            }

                            Column
                            {
                                id: analysesColumn
                                anchors.top: parent.top
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.topMargin: Regovar.theme.font.boxSize.header + 5
                                anchors.rightMargin: 15
                                onHeightChanged: analysesScrollArea.height = Math.max(height + Regovar.theme.font.boxSize.header + 5, analysesScrollArea.height)

                                Repeater
                                {
                                    model: regovar.lastAnalyses
                                    SearchResultAnalysis
                                    {
                                        indent: 0
                                        width: analysesScrollArea.width - 30 // 15 right margin + 15 ScrollBar width
                                        date: model.modelData.update_date
                                        name: model.modelData.name
                                        projectName: model.modelData.project.name

                                        onClicked: regovar.openAnalysis(model.modelData.id)
                                        anchors.left: analysesColumn.left
                                        anchors.right: analysesColumn.right
                                    }
                                }
                            }
                        }

                        Rectangle
                        {
                            id: subjectScrollArea
                            color: "transparent"
                            // height is sized by colomn content
                            onHeightChanged: row.height = Math.max(height, row.height)
                            clip: true

                            Text
                            {
                                id: subjectsHeader
                                anchors.left: parent.left
                                anchors.leftMargin: 15
                                verticalAlignment: Text.AlignVCenter
                                elide: Text.ElideRight
                                font.pixelSize: Regovar.theme.font.size.header
                                color: Regovar.theme.primaryColor.back.dark
                                height: Regovar.theme.font.boxSize.header
                                text: qsTr("Last subjects")
                            }

                            Rectangle
                            {
                                anchors.top: subjectsHeader.bottom
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.leftMargin: 15
                                height: 1
                                color: Regovar.theme.primaryColor.back.normal
                            }

                            Column
                            {
                                anchors.fill: parent
                                anchors.topMargin: Regovar.theme.font.boxSize.header + 5
                                anchors.rightMargin: 15
                                onHeightChanged: subjectScrollArea.height = Math.max(height + Regovar.theme.font.boxSize.header + 5, subjectScrollArea.height)

                                Repeater
                                {
                                    model: regovar.lastSubjects
                                    SearchResultAnalysis
                                    {
                                        width: 500
                                        date: model.modelData.update_date
                                        name: model.modelData.name
                                        projectName: model.modelData.project.name

                                        onClicked: regovar.loadAnalysis(model.modelData.id)
                                    }
                                }
                            }
                        }
                    }
                }



                Text
                {
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                    font.pixelSize: Regovar.theme.font.size.header
                    color: Regovar.theme.primaryColor.back.dark
                    height: Regovar.theme.font.boxSize.header
                    text: qsTr("Last events")
                }

                Rectangle
                {
                    width: scrollViewArea.width
                    height: 1
                    color: Regovar.theme.primaryColor.back.normal
                }


                Column
                {
                    Repeater
                    {
                        model : ListModel
                        {
                            ListElement { date: "2017-06-25 14h56"; name: "Article published"; icon:""; color:"" }
                            ListElement { date: "2017-06-25 14h56"; name: "Start new analysis \"Hugodims\""; icon:""; color:"" }
                            ListElement { date: "2017-06-25 14h56"; name: "Pause analysis \"Hugodims\""; icon:"m"; color:"red" }
                            ListElement { date: "2017-06-25 14h56"; name: "Project 2 creation"; icon:""; color:"" }
                        }

                        Row
                        {
                            Text
                            {
                                width: 120
                                font.pixelSize: 12
                                font.family: Regovar.theme.font.familly
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.left
                                text: date
                                color: Regovar.theme.frontColor.disable
                            }
                            Text
                            {
                                width: 30
                                font.pixelSize: 12
                                font.family: Regovar.theme.icons.name
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                text: icon
                                color: color
                            }
                            Text
                            {
                                font.pixelSize: 12
                                font.family: Regovar.theme.font.familly
                                color: Regovar.theme.frontColor.normal
                                verticalAlignment: Text.AlignVCenter
                                text: name
                            }
                        }
                    }
                }
            }
        }


        BusyIndicator
        {
            visible: !regovar.welcomIsLoading
            anchors.top: newButtonsRow.bottom
            anchors.topMargin: 70
            anchors.left: panel.left
            anchors.right: panel.right
            anchors.bottom: panel.bottom

        }


    }

    // DIALOGS
    NewProjectDialog { id: newProjectDialog }

    NewAnalysisDialog { id: newAnalysisDialog }

    NewSubjectDialog { id: newSubjectDialog }
}

