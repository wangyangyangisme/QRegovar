import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls 1.4
import Regovar.Core 1.0

import "../../Regovar"
import "../../Framework"

Rectangle
{
    id: root
    color: Regovar.theme.backgroundColor.main

    property QtObject model

    property bool isEmpty: true
    property bool isLoading: false

    File { id: fileInstance }

    Connections
    {
        target: regovar
        onSearchResultChanged:
        {
            console.log("search result : " + regovar.searchResult["total_result"]);
            isEmpty = regovar.searchResult["total_result"] === 0;
            searchResults.displayresults(regovar.searchResult);
        }
    }


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
            id: connectionStatus
            anchors.top: header.top
            anchors.right: header.right
            anchors.bottom: header.bottom
            anchors.margins: 5
            anchors.rightMargin: 10
        }

        TextField
        {
            anchors.top: header.top
            anchors.left: header.left
            anchors.bottom: header.bottom
            anchors.right: connectionStatus.left
            anchors.margins: 10

            property string formerSearch: ""
            iconLeft: "z"
            text: regovar.searchRequest
            placeholder: qsTr("Search subjects, samples, analyses, panels...")
            onEditingFinished:
            {
                if (formerSearch != text)
                {
                    regovar.search(text);
                    formerSearch = text;
                }
            }
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
        text: qsTr("Use the field above to search everything in Regovar. Then double click on the result below to open it and see details.")
    }

    Rectangle
    {
        id: empty
        visible: isEmpty
        anchors.top : header.bottom
        anchors.left: root.left
        anchors.right: root.right
        anchors.bottom: root.bottom
        anchors.margins: 10
        anchors.topMargin: Regovar.helpInfoBoxDisplayed ? helpInfoBox.height + 20 : 10
        color: "transparent"

        Text
        {
            anchors.centerIn: parent
            text: "No result"
            font.pixelSize: Regovar.theme.font.size.title
            color: Regovar.theme.primaryColor.back.light
        }
    }


    Rectangle
    {
        id: resultsList
        visible: !isEmpty
        anchors.top : header.bottom
        anchors.left: root.left
        anchors.right: root.right
        anchors.bottom: root.bottom
        anchors.margins: 10
        anchors.topMargin: Regovar.helpInfoBoxDisplayed ? helpInfoBox.height + 20 : 10


        color: "transparent"

        Text
        {
            anchors.top: parent.top
            text: regovar.searchResult["total_result"] + " " + ( (regovar.searchResult["total_result"] > 1 ) ? qsTr("results found") : qsTr("result found"))
            font.pixelSize: Regovar.theme.font.size.header
            height: Regovar.theme.font.boxSize.header
            color: Regovar.theme.primaryColor.back.dark
        }

        SearchResultsList
        {
            id: searchResults
            anchors.fill: parent
            anchors.topMargin: Regovar.theme.font.boxSize.title + 5
        }

        Rectangle
        {
            anchors.left: resultsList.left
            anchors.right: resultsList.right
            anchors.bottom: searchResults.top
            height: 1
            color: Regovar.theme.primaryColor.back.normal
        }
    }



    Rectangle
    {
        id: loading
        visible: regovar.searchInProgress
        z: 100
        anchors.top : header.bottom
        anchors.left: root.left
        anchors.right: root.right
        anchors.bottom: root.bottom
        anchors.margins: 10
        anchors.topMargin: Regovar.helpInfoBoxDisplayed ? helpInfoBox.height + 20 : 10

        color: Regovar.theme.backgroundColor.main

        BusyIndicator
        {
            anchors.centerIn: parent
        }
    }
}
