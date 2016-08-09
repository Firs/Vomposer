/**************************************************************************
** Copyright (c) 2016 Fedor Eliseev <feliseev@gmail.com>.
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as
** published by the Free Software Foundation, version 3.
**
** This program is distributed in the hope that it will be useful, but
** WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
** Lesser General Lesser Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with this program. If not, see <http://www.gnu.org/licenses/>.
**************************************************************************/
import QtQuick 2.0
import QtQuick.Window 2.2

Window {
    visible: true
    width: 480
    height: 640
    title: qsTr("Vomposer")

    Rectangle {
        id: rectangle2
        anchors.fill: parent
        color: "#212126"

        Item {
            id: noteBar
            anchors.fill: parent
            visible: PitchMonitor.Frequency > 0.0

            Text {
                id: noteName
                font.pixelSize: parent.height / 6
                anchors.verticalCenter: parent.verticalCenter
                x: parent.width / 2 - font.pixelSize / 2
                text: PitchMonitor.PitchClass
                color: "white"
            }

            Text {
                id: noteSharp
                text: "#"
                font.pixelSize: noteName.height / 2.2
                anchors.top: noteName.top
                x: noteName.x + noteName.font.pixelSize * 0.8
                color: "white"
                visible: PitchMonitor.PitchSharp
            }

            Text {
                id: noteOctave
                text: PitchMonitor.PitchOctave
                font.pixelSize: noteName.height / 2.2
                anchors.bottom: noteName.bottom
                x: noteName.x + noteName.font.pixelSize * 0.8
                color: "white"
            }

            FrequencyBar {
                width: noteName.font.pixelSize
                height: noteName.font.pixelSize
                anchors.verticalCenter: parent.verticalCenter
                x: noteName.x - width * 1.2
            }
        }



//        Image {
//            id: trebleClef
//            anchors.bottomMargin: 10
//            fillMode: Image.Stretch
//            anchors.bottom: parent.bottom

//            sourceSize.width: 200
//            sourceSize.height: parent.height / 5

//            source: "Treble_clef.svg"

//        }



    }
}
