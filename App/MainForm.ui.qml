import QtQuick 2.7

Rectangle {
    property alias mouseArea: mouseArea
    property alias noteNameLabel: noteNameLabel

    MouseArea {
        id: mouseArea
        anchors.fill: parent
    }

    Text {
        id: noteNameLabel
        text: "C#4"
        font.pointSize: 72
        anchors.centerIn: parent
    }
}
