import QtQuick 2.7
import QtQuick.Controls 1.4

Item
{
    id: menuWrapper
    anchors.fill: parent

    property Rectangle mw

    MenuBar
    {
	id: menuBar
	Menu
	{
	    title: "File"
	    MenuItem { text: "First" }
	    MenuItem { text: "Second" }
	    MenuItem { text: "Third" }
	}

    }

    states: State
    {
	name: "hasMenuBar"
	when: menuBar && !menuBar.__isNative

	ParentChange
	{
	    target: menuBar.__contentItem
	    parent: mw
	}

	PropertyChanges
	{
	    target: menuBar.__contentItem
	    x: 0
	    y: 0
	    width: menuWrapper.width
	}
    }
}
