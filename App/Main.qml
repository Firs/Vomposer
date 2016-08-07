import QtQuick 2.7
import QtQuick.Window 2.2

Window {
    visible: true
    width: 480
    height: 640
    title: qsTr("Vomposer")

    Rectangle {
        anchors.fill: parent
        color: "#212126"

        MouseArea {
            id: mouseArea
            anchors.fill: parent
        }

        Text {
            font.pointSize: 72
            anchors.centerIn: parent
            text: PitchMonitor.PitchName
            color: "white"
        }
    }
}
