import QtQuick 2.7
import QtQuick.Window 2.2
import Vomposer 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Vomposer")

    MainForm {
        anchors.fill: parent

        PitchValueSource on noteNameLabel.text {}
    }
}
