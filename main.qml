import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Window 2.0
import OpenGLUnderQML 1.0

Window {
    visible: true
    width: 1280
    height: 720
    title: qsTr("OpenGL Demo")
    property int durationValue : 1000 + 100 / slider.value
    property int speed: slider.value * 100

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Squircle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            SequentialAnimation on t {
                id: squircleAnimation
                NumberAnimation { to: 1; duration: durationValue; easing.type: Easing.InQuad }
                NumberAnimation { to: 0; duration: durationValue; easing.type: Easing.OutQuad }
                loops: Animation.Infinite
                running: true
            }
        }

        RowLayout {
            spacing: 10
            Slider {
                id: slider
                Layout.fillWidth: true
                from: 0.1
                to: 1
                value: 0.5
                onValueChanged: {
                    squircleAnimation.stop()
                    squircleAnimation.start()
                }
            }
            Text {
                text: qsTr("Speed") + ": " + speed
            }
        }
    }
}
