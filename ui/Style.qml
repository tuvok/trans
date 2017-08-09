pragma Singleton
import QtQuick 2.7

QtObject
{
    property color background: "#444444"
    property color lightBackground: "#555555"
    property color veryLightBackground: "#DDDDDD"

    property QtObject button: QtObject
    {
        property int width: 32
        property int height: 32
        property int imageWidth: 24
        property color textColor: "#000000"
	
        property QtObject border: QtObject
        {
            property color color: "#222222"
            property int radius: 3	    
        }
	
        property Gradient gradNotPressed: Gradient
        {
            GradientStop { position: 0 ; color: "#555555" }
            GradientStop { position: 1 ; color: "#444444" }
        }

        property Gradient gradPressed: Gradient
        {
            GradientStop { position: 0 ; color: "#444444" }
            GradientStop { position: 1 ; color: "#333333" }
        }
    }

    property QtObject icons: QtObject
    {
        property url documentImport: "/usr/share/icons/Humanity/actions/48/document-import.svg"
    }
}
