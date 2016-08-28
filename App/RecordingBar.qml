import QtQuick 2.0

Item {
    property var numKeys: 36
    property var keyWidth: 24
    property var keyHeight: 12
    property var noteSize: 12
    property var topOctave: 4
    property var backgroundColor: "#212126"
    property var gridColor: "#303030"
    property var highlightColor: "orange"

    Rectangle {
        anchors.fill: parent
        color: backgroundColor

        // Piano roll
        Rectangle {
            id: pianoRoll
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: keyWidth

            Column {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom

                Repeater {
                    model: 3

                        Column {
                            Rectangle {
                                width: keyWidth
                                height: keyHeight
                                color: "white"
                                border.color: gridColor
                            }
                            Rectangle {
                                width: keyWidth
                                height: keyHeight
                                color: "black"
                                border.color: gridColor
                            }
                            Rectangle {
                                width: keyWidth
                                height: keyHeight
                                color: "white"
                                border.color: gridColor
                            }
                            Rectangle {
                                width: keyWidth
                                height: keyHeight
                                color: "black"
                                border.color: gridColor
                            }
                            Rectangle {
                                width: keyWidth
                                height: keyHeight
                                color: "white"
                                border.color: gridColor
                            }
                            Rectangle {
                                width: keyWidth
                                height: keyHeight
                                color: "black"
                            }
                            Rectangle {
                                width: keyWidth
                                height: keyHeight
                                color: "white"
                                border.color: gridColor
                            }
                            Rectangle {
                                width: keyWidth
                                height: keyHeight
                                color: "white"
                                border.color: gridColor
                            }
                            Rectangle {
                                width: keyWidth
                                height: keyHeight
                                color: "black"
                                border.color: gridColor
                            }
                            Rectangle {
                                width: keyWidth
                                height: keyHeight
                                color: "white"
                                border.color: gridColor
                            }
                            Rectangle {
                                width: keyWidth
                                height: keyHeight
                                color: "black"
                                border.color: gridColor
                            }
                            Rectangle {
                                width: keyWidth
                                height: keyHeight
                                color: "white"
                                border.color: gridColor
                                Text {
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    font.pixelSize: 10
                                    text: "C" + (topOctave - index)
                                }
                            }

                    }
                }
            }
        }

        Rectangle {
            anchors.left: pianoRoll.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right

            color: parent.color

            Grid {
                id: notesPane
                anchors.fill: parent
                columns: 40
                rows: numKeys
                spacing: 0
                Repeater {
                    model: numKeys * 40
                    Rectangle {
                        width: noteSize
                        height: noteSize
                        border.color: gridColor
                        border.width: 1
                        color: (index >= 160 && index < 200) ? highlightColor : backgroundColor
                    }
                }
            }
        }
    }
}
