import QtQuick
import QtQuick.Controls

Rectangle {
    width: 1920
    height: 1080
    visible: true
    color: "black"

    Image {
        //Hintergrundbild
        id: backgroundImage
        source: "qrc:/bilder/common/background.png"
        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop
    }
    Image {
        //IAV-Logo
        id: iavLogo
        source: "qrc:/bilder/common/iav-logo.svg"
        scale: 0.2
        anchors.centerIn: parent
        anchors.verticalCenterOffset: -500
        fillMode: Image.PreserveAspectCrop
    }
    Text {
        //Wartetext während des Wartevorgangs
        id: connectionText
        anchors.centerIn: parent
        text: "Verbindung zum Tuner wird aufgebaut..."
        font.pixelSize: 40
        color: "white"
        visible: !radio_HAL.connection //Sichtbar falls keine Verbindung ist
    }
    SwipeView {
        //Radioinhalt
        id: swipeView
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
        visible: radio_HAL.connection //Sichtbar falls Verbindung da ist
        Loader {
            source: "qrc:/qml/radio.qml"
        }
    }
}
