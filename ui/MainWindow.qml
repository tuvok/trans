import QtQuick 2.7

import QtQuick.Controls 2.0
import QtQuick.Controls 1.4 as OldControls

import QtQuick.Dialogs 1.2 // file dialog

import "."

import org.qt.qtextedit 1.0

Rectangle {
    id: mainWindow
    visible: true

    color: Style.background

//    x: 0
//    y: 0
    width: 800
    height: 600

    ToolButton
    {
        id: importFileButton
        iconSource: Style.icons.documentImport
        
        onClicked: importFileDialog.open()
    }

    FileDialog
    {
        id: importFileDialog
		objectName: "importFileDialog"
		nameFilters: [ "Tex files (*.tex)" ]
		
		signal accept(url filePath, QtObject textDocument)

		onAccepted: accept(fileUrl, texPreviewArea.textDocument)
    }

    Rectangle
    {
        id: texImportPreview
        objectName: "texImportPreview"
        color: Style.background

        opacity: 0.0
        visible: false
        
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        width: parent.width
        height: parent.height
        
        signal accept(int id, string collection)
        signal decline(int id)
        signal provideCollections(int level, string parent)
        
        property int currentPreviewId
        
        function showTexImportPreview(id)
        {
            currentPreviewId = id
            animateOpacityShow.start()
        }

        function closeTexImportPreview(accepted)
        {
            if (accepted)
            {
                var colName = c1.visible && c1.currentText ? c1.currentText : ""
                colName += c2.visible && c2.currentText ? "." + c2.currentText : ""
                colName += c3.visible && c3.currentText ? "." + c3.currentText : ""
                colName += c4.visible && c4.currentText ? "." + c4.currentText : ""
                colName += collectionName.visible && collectionName.text ? "." + collectionName.text : ""

                accept(currentPreviewId, colName)
            }
            else
                decline(currentPreviewId)
                
            animateOpacityHide.start()
        }
        
        function setCollections(level, collections)
        {
            if (level === 0)
            {
                c1.model = collections
                c1.currentIndex = 0
                c1.visible = true
            }
            if (level === 1)
            {
                c2.model = collections
                c2.currentIndex = 0
                c2.visible = true
            }
            if (level === 2)
            {
                c3.model = collections
                c3.currentIndex = 0
                c3.visible = true
            }
            if (level === 3)
            {
                c4.model = collections
                c4.currentIndex = 0
                c4.visible = true
            }
            collectionName.text = ""
        }

        
        MouseArea
        {
            anchors.fill: parent
        }
        
        Item
        {
            id: texPreviewItem
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.topMargin: 10
            width: parent.width - 20
            height: parent.height - collectionSelectorFrame.height - buttonFrame.height * 2
            
            Rectangle
            {
                anchors.fill: parent
                color: Style.lightBackground
                radius: 5
                border.color: "#505050"
            }
        
            OldControls.ScrollView
            {
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: 5
                
                width: parent.width - 10
                height: parent.height
                
                TextEdit
                {
                    id: texPreviewArea
                    width: texPreviewItem.width - 40
                    
                    readOnly: true
                    wrapMode: TextEdit.Wrap
                }
            }
        }
        Item
        {
            id: collectionSelectorFrame
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: buttonFrame.top
            anchors.bottomMargin: 10
            
            width: (c1.visible ? c1.width : 0) + (c2.visible ? c2.width + 5 : 0) + (c3.visible ? c3.width + 5 : 0) + (c4.visible ? c4.width + 5 : 0) + (collectionNameFrame.visible ? collectionNameFrame.width + 15 : 0)
            height: 40
            
            ComboBox
            {
                id: c1
                visible: false
                
                onCurrentTextChanged: {
                    if (currentText)
                        texImportPreview.provideCollections(1, currentText)
                    else
                    {
                        c2.visible = false
                        c2.currentIndex = 0
                    }
                }
            }
            ComboBox
            {
                id: c2
                visible: false

                anchors.left: c1.visible ? c1.right : undefined
                anchors.leftMargin: 5
                
                onCurrentTextChanged: {
                    if (currentText)
                        texImportPreview.provideCollections(2, currentText)
                    else
                    {
                        c3.visible = false
                        c3.currentIndex = 0
                    }
                }
            }
            ComboBox
            {
                id: c3
                visible: false

                anchors.left: c2.visible ? c2.right : ( c1.visible ? c1.right : undefined)
                anchors.leftMargin: 5
                
                onCurrentTextChanged: {
                    if (currentText)
                        texImportPreview.provideCollections(3, currentText)
                    else
                    {
                        c4.visible = false
                        c4.currentIndex = 0
                    }
                }
            }
            ComboBox
            {
                id: c4
                visible: false

                anchors.left: c3.visible ? c3.right : ( c2.visible ? c2.right : (c1.visible ? c1.right : undefined))
                anchors.leftMargin: 5
            }
            Item
            {
                id: collectionNameFrame
                width: c4.width * 2
                height: c4.height
                anchors.left: c4.visible ? c4.right : (c3.visible ? c3.right : ( c2.visible ? c2.right : (c1.visible ? c1.right : undefined)))
                anchors.leftMargin: 15
                visible: true
                
                Rectangle
                {
                    anchors.fill: parent
                    color: Style.veryLightBackground
                }
                
                Flickable
                {
                    id: flick

                    width: parent.width
                    height: parent.height
                    contentWidth: collectionName.paintedWidth
                    contentHeight: collectionName.paintedHeight
                    anchors.verticalCenter: parent.verticalCenter
                    
                    clip: true

                    function ensureVisible(r)
                    {
                        if (contentX >= r.x)
                            contentX = r.x;
                        else if (contentX+width <= r.x+r.width)
                            contentX = r.x+r.width-width;
                        if (contentY >= r.y)
                            contentY = r.y;
                        else if (contentY+height <= r.y+r.height)
                            contentY = r.y+r.height-height;
                    }
                    TextEdit
                    {
                        id: collectionName
                        visible: true
                        selectByMouse: true
                        verticalAlignment: TextEdit.AlignVCenter
                                            
                        width: flick.width
                        height: flick.height
                        onCursorRectangleChanged: flick.ensureVisible(cursorRectangle)
                        
                        font.pointSize: 12
                    }
                }
            }
        }
        Item
        {
            id: buttonFrame
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            width: saveButton.width + 10 + cancelButton.width
            height: saveButton.height
            ToolButton
            {
                id: saveButton
                text: "Save"
                width: 80
                
                anchors.left: parent.left
                anchors.top: parent.top
                
                onClicked: texImportPreview.closeTexImportPreview(true)
            }
            ToolButton
            {
                id: cancelButton
                text: "Cancel"
                width: 80
                
                anchors.left: saveButton.right
                anchors.leftMargin: 10
                
                onClicked: texImportPreview.closeTexImportPreview(false)
            }
        }
        NumberAnimation
        {
            id: animateOpacityShow
            target: texImportPreview
            properties: "opacity"
            from: 0.0
            to: 1.0
            duration: 400
            onStarted: texImportPreview.visible = true
        }
        NumberAnimation
        {
            id: animateOpacityHide
            target: texImportPreview
            properties: "opacity"
            from: 1.0
            to: 0.0
            duration: 400
            onStopped: texImportPreview.visible = false
        }
        Connections
        {
            target: Importer
            onShowTexImportPreview: texImportPreview.showTexImportPreview(id)
            onSetCollectionCombos: texImportPreview.setCollections(level, collections)
        }
    }


}
