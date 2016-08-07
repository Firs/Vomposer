import QtQuick 2.7

Item {
    Rectangle {
        color: "#212126"
        anchors.fill: parent

        Rectangle {
            id: nextPitchBar
            width: parent.width
            height: 2
            anchors.top: parent.top
            color: "grey"
            radius: height
        }

        Rectangle {
            id: currPitchBar
            width: parent.width
            height: 2
            anchors.verticalCenter: parent.verticalCenter
            color: "grey"
            radius: height
        }

        Rectangle {
            id: prevPitchBar
            width: parent.width
            height: 2
            color: "grey"
            anchors.bottom: parent.bottom
            radius: height
        }

        Rectangle {
            function calcFrequencyBarY() {
                var halfHeight = parent.height / 2
                var centerPos = halfHeight - height / 2;

                var nextPitchFreq = PitchMonitor.NextPitchFrequency;
                var prevPitchFreq = PitchMonitor.PrevPitchFrequency;

                if (nextPitchFreq <= prevPitchFreq)
                    return centerPos;

                var currPitchFreq = PitchMonitor.PitchFrequency;
                var currFreq = PitchMonitor.Frequency;

                if (currFreq >= currPitchFreq) {
                    // Top half
                    var range = nextPitchFreq - currPitchFreq;
                    var percent = (currFreq - currPitchFreq) / range;
                    return centerPos - parent.height * percent;
                }
                else {
                    // Bottom half
                    var range = currPitchFreq - prevPitchFreq;
                    var percent = (currFreq - prevPitchFreq) / range;
                    return centerPos + parent.height - parent.height * percent;
                }
            }

            id: currFrequencyBar
            width: parent.width
            height: 4
            visible: PitchMonitor.Frequency > 0.0
            y: calcFrequencyBarY()
            radius: height

            Connections {
                target: PitchMonitor
                onPitchChanged: {

                    if (Math.abs(PitchMonitor.Frequency - PitchMonitor.PitchFrequency) < 1) {
                        currFrequencyBar.color = "green"
                    }
                    else {
                        currFrequencyBar.color = "yellow"
                    }
                }
            }

        }
    }
}
