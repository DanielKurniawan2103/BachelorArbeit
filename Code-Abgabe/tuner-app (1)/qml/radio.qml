import QtQuick
import QtQuick.Controls 2.2
import RADIO_METHODS 1.0

Item {
    visible: true
    Rectangle {

        width: parent.width
        height: parent.height - 150
        anchors.centerIn: parent
        color: "transparent"
        radius: 30

        Text {
            id: name
            anchors.centerIn: parent
            anchors.horizontalCenterOffset: -280
            anchors.verticalCenterOffset: -350
            text: radio_HAL.name
            font.pixelSize: 50
            font.bold: true
            color: "white"
        }
        Text {
            id: frequency
            anchors.centerIn: parent
            anchors.horizontalCenterOffset: -300
            anchors.verticalCenterOffset: -270
            text: radio_HAL.frequency / 1000
            font.pixelSize: 40
            color: "white"
            visible: {
                if (radio_HAL.frequency == 0) {
                    return false
                } else {
                    return true
                }
            }
        }
        Text {
            id: band
            anchors.centerIn: parent
            anchors.horizontalCenterOffset: -280
            anchors.verticalCenterOffset: -180
            text: {
                var fullText = radio_HAL.band
                if (fullText.length > 40) {
                    return fullText.substring(0, 40) + "..."
                } else {
                    return fullText
                }
            }
            font.pixelSize: 40
            color: "white"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        Text {
            id: info
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -180
            anchors.horizontalCenterOffset: -120
            text: radio_HAL.info
            font.pixelSize: 30
            color: "white"
        }

        Image {
            id: stationImage
            anchors.centerIn: parent
            anchors.horizontalCenterOffset: -720
            anchors.verticalCenterOffset: -250
            width: 280
            height: 280
            fillMode: Image.PreserveAspectFit
            cache: false
            source: radio_HAL.imageUrl.toString()
        }

        Image {
            id: currentSongImage
            anchors.centerIn: parent
            anchors.horizontalCenterOffset: -720
            anchors.verticalCenterOffset: 0
            width: 200
            height: 200
            fillMode: Image.PreserveAspectFit
            cache: false
            source: radio_HAL.currentImageUrl.toString()
        }

        Rectangle {
            id: song
            anchors.centerIn: parent
            anchors.horizontalCenterOffset: -300
            width: 300
            height: 100
            color: "transparent"
            ScrollView {
                id: view
                width: parent.width
                anchors.fill: parent
                clip: true

                Flickable {
                    id: flickable
                    width: parent.width
                    height: parent.height
                    contentWidth: label.width
                    contentHeight: parent.height
                    interactive: false

                    Label {
                        id: label
                        text: radio_HAL.song
                        font.pixelSize: 30
                        color: "white"
                        anchors.verticalCenter: parent.verticalCenter

                        wrapMode: Text.NoWrap
                    }
                }
            }

            Timer {
                id: scrollTimer
                interval: 50
                running: true
                repeat: true
                onTriggered: {

                    if (flickable.contentX < flickable.contentWidth - view.width) {
                        flickable.contentX += 1
                    } else {

                        scrollTimer.stop()
                        resetTimer.start()
                    }
                }
            }
            Timer {
                id: resetTimer
                interval: 1000 // 1 second delay
                running: false
                repeat: false
                onTriggered: {
                    flickable.contentX = 0
                    scrollTimer.start()
                }
            }
        }

        Button {
            id: downFineButton
            anchors.centerIn: parent
            anchors.horizontalCenterOffset: -700
            anchors.verticalCenterOffset: 200
            height: 125
            width: 250
            background: Rectangle {
                color: "transparent"
            }

            contentItem: Image {
                id: downFineButtonImage
                source: "qrc:/bilder/common/buttons/2-1/left-top.svg"
            }

            Text {
                text: qsTr("<<")
                anchors.centerIn: parent
                color: "white"
                font.pixelSize: 50
            }

            MouseArea {
                anchors.fill: parent
                onPressed: {
                    downFineButtonImage.source
                            = "qrc:/bilder/common/buttons/2-1/left-top-selected.svg"
                }
                onClicked: {
                    var nextIndex = (listView.currentIndex - 1) % listView.count
                    if (nextIndex > 0) {
                        listView.currentIndex = nextIndex
                        var nextItem = listView.itemAtIndex(nextIndex)
                        if (nextItem) {
                            nextItem.clicked()
                        }
                    }
                }
                onReleased: {
                    downFineButtonImage.source = "qrc:/bilder/common/buttons/2-1/left-top.svg"
                }
                onCanceled: {
                    downFineButtonImage.source = "qrc:/bilder/common/buttons/2-1/left-top.svg"
                }
            }
        }

        Button {
            id: upFineButton
            anchors.centerIn: parent
            anchors.horizontalCenterOffset: -200
            anchors.verticalCenterOffset: 200
            height: 125
            width: 250
            background: Rectangle {
                color: "transparent"
            }

            contentItem: Image {
                id: upFineButtonImage
                source: "qrc:/bilder/common/buttons/2-1/right-top.svg"
            }

            Text {
                text: qsTr(">>")
                anchors.centerIn: parent
                color: "white"
                font.pixelSize: 50
            }

            MouseArea {
                anchors.fill: parent
                onPressed: {
                    upFineButtonImage.source
                            = "qrc:/bilder/common/buttons/2-1/right-top-selected.svg"
                }
                onClicked: {
                    console.log("Up Button Pressed")
                    var nextIndex = (listView.currentIndex + 1) % listView.count
                    console.log("Index: ", nextIndex)
                    listView.currentIndex = nextIndex
                    var nextItem = listView.itemAtIndex(nextIndex)
                    if (nextItem) {
                        nextItem.clicked()
                    }
                }

                onReleased: {
                    upFineButtonImage.source = "qrc:/bilder/common/buttons/2-1/right-top.svg"
                }
                onCanceled: {
                    upFineButtonImage.source = "qrc:/bilder/common/buttons/2-1/right-top.svg"
                }
            }
        }

        Row {
            id: memoryButtonsRow
            spacing: 30
            anchors.centerIn: parent
            anchors.horizontalCenterOffset: -450
            anchors.verticalCenterOffset: 400
            Repeater {
                model: 5
                delegate: Button {
                    id: memoryButton
                    width: 130
                    height: 130

                    background: Rectangle {
                        id: memoryButtonBackground
                        width: parent.width
                        height: parent.height
                        color: "transparent"
                        border.color: "lightblue"
                        radius: 20
                    }
                    Image {
                        id: memoryButtonImage
                        width: 100
                        height: 100
                        source: "file:" + radio_HAL.memoryStations[index].name + ".png"
                        anchors.centerIn: parent

                        onStatusChanged: {
                            console.log("Loading image: " + source)
                            if (status == Image.Error) {
                                source = "qrc:/bilder/icons/radio.PNG"
                            }
                        }
                    }

                    MouseArea {
                        id: mouseArea
                        anchors.fill: parent
                        onPressed: {
                            memoryButtonBackground.border.color = "blue"
                        }

                        onClicked: {
                            radio_HAL.loadMemoryStation(index)
                            console.log("Memory " + (index + 1) + " clicked")
                        }
                        onPressAndHold: {
                            radio_HAL.setMemoryStation(index)
                            console.log("Memory " + (index + 1) + " set")
                            memoryButtonImage.source = "file:"
                                    + radio_HAL.memoryStations[index].name + ".png"
                            memoryButtonBackground.border.color = "blue"
                        }

                        onReleased: {
                            memoryButtonBackground.border.color = "lightblue"
                        }
                        onCanceled: {
                            memoryButtonBackground.border.color = "lightblue"
                        }
                    }
                }
            }
        }

        ListView {
            id: listView
            width: parent.width / 2.3
            height: parent.height - 100
            anchors.centerIn: parent
            anchors.horizontalCenterOffset: 450
            anchors.verticalCenterOffset: 100
            spacing: 5

            model: radioStationModel //Model für die Radiostation
            delegate: Button {       //Jede Radiostation wird als ein Knopf dargestellt
                id: stationMemoryButton
                width: ListView.view.width
                height: 130

                background: Rectangle {
                    id: buttonBackground
                    width: parent.width
                    height: parent.height
                    color: "transparent"
                    border.color: "lightblue"
                    radius: 20
                }
                Timer {
                    id: resetColorTimer
                    interval: 100 // 1 second
                    repeat: true
                    onTriggered: {
                        stationMemoryButton.released()
                        stationMemoryButton.canceled()
                    }
                }

                onPressed: {
                    buttonBackground.border.color = "blue"
                    resetColorTimer.start()
                }

                onClicked: {
                    //Informationen über die aktuelle Radiostation wird
                    radio_HAL.song = model.song
                    radio_HAL.band = model.band
                    radio_HAL.frequency = model.frequency
                    radio_HAL.primaryId = model.primaryId
                    radio_HAL.secondaryIds = model.secondaryIds
                    radio_HAL.info = model.info
                    radio_HAL.name = model.name
                    listView.currentIndex = index //Der Index für die Step-Knöpfe wird aktuallisiert
                }

                onReleased: {
                    buttonBackground.border.color = "lightblue"
                }
                onCanceled: {
                    buttonBackground.border.color = "lightblue"
                }

                Text {
                    text: {
                        var fullText = model.name
                        if (fullText.length > 30) {
                            return fullText.substring(0, 30) + "..."
                        } else {
                            return fullText
                        }
                    }

                    font.bold: true
                    color: "white"
                    font.pixelSize: 40
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                }

                Text {
                    text: model.band
                    color: "white"
                    font.pixelSize: 40
                    anchors.centerIn: parent

                    anchors.horizontalCenterOffset: 180
                }

                Image {
                    width: 100
                    height: 100
                    source: "file:" + model.name + ".png"
                    anchors.centerIn: parent
                    anchors.horizontalCenterOffset: 330
                }
            }
        }
    }
}
