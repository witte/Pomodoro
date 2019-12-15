import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Dialogs 1.1
import QtMultimedia 5.14
import io.github.witte 1.0 as C

/*
    THINGS TO HAVE:
        - settings
            - view reports
            - about page with:
                - made with Qt statement
                - link to gibhub page
        - "go to break/pomodoro now" (shortens pomodoro/break time)
        - "3 more minutes" (extends pomodoro/break time)
        - save start/stop/go to's/3 more's timetags as JSON (SQLite?) files
        - generate reports from JSON (SQLite?) files
*/

ApplicationWindow
{
    id: app
    visible: true
    width: 640
    minimumHeight: 200
    minimumWidth: 200
    height: 480
    title: qsTr ("Pomodoro")
    color: mainBgColor

    property double sizeRate: ((height > width) ? app.width : app.height) * 0.92
    property string state: ""

    property string mainColor: stoppedColor
    property string mainBgColor: "#e9eaeb"
    property string stoppedColor: "#65697e"
    property string pomodoroColor: "#aa0000"
    property string breakColor: "#336632"

    property int pomodoroLength: 25 // pomodoro time in minutes
    property int breakLength:    5 // break time in minutes
    property double positionOffset: 0
    property double elapsedTimeOffset: 0
    property date lastStartTime: new Date()
    property date nextEndTime: new Date()

    Behavior on positionOffset
    {
        PropertyAnimation { duration: 1000; easing.type: Easing.InOutQuart }
    }

    Item
    {
        states:
        [
            State
            {
                when: (app.state === "") //stopped
                PropertyChanges { target: app; mainColor: app.stoppedColor }
                PropertyChanges { target: textLabel; text: qsTr ("start") }
            },
            State
            {
                when: (app.state === "running")
                PropertyChanges { target: app; mainColor: app.pomodoroColor }
                PropertyChanges { target: textLabel; text: qsTr ("stop") }
            },
            State
            {
                when: (app.state === "break")
                PropertyChanges { target: app; mainColor: app.breakColor  }
                PropertyChanges { target: textLabel; text: qsTr ("stop") }
            }
        ]
    }

    Item
    {
        id: itemRef
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter:   parent.verticalCenter
        width:  app.sizeRate
        height: app.sizeRate

        Rectangle
        {
            anchors.fill: button
            anchors.margins: app.sizeRate * 0.015
            radius: button.height / 2
            color: mainBgColor
            border.color: app.mainColor
            border.width: 1
        }

        MouseArea
        {
            id: button
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            width: app.sizeRate * 0.98
            height: width

            onClicked:
            {
                if (app.state !== "")
                {
                    app.positionOffset = 0
                    app.elapsedTimeOffset = 0
                    sliceElapsedTime.toAngle = 0

                    app.state = ""
                    timer.stop()
                }
                else
                {
                    app.lastStartTime = new Date ()
                    app.nextEndTime = new Date (new Date().getTime() + (app.pomodoroLength * 60000))
                    sliceElapsedTime.toAngle = slicePomodoroLength.fromAngle

                    app.state = "running"
                    timer.start()
                }
            }

            C.Pie
            {
                id: lengthsPie
                anchors.fill: parent

                C.PieSlice
                {
                    property string baseColor: app.mainColor

                    id: slicePomodoroLength
                    anchors.fill: parent
                    fromAngle: 6 * app.positionOffset
                    toAngle: 6 * (app.positionOffset + app.pomodoroLength)
                    color: containsMouse ? Qt.lighter (baseColor, 1.2) : baseColor

                    onClicked: button.clicked (event)
                    onMoved:
                    {
                        let time = angle / 6
                        if ((time < 1) || (time + app.breakLength > 59)) return

                        app.pomodoroLength = time
                        textLabel.text = app.pomodoroLength + "min"
                    }
                    onReleased: textLabel.text = "start"
                }

                C.PieSlice
                {
                    id: sliceBreakLength
                    anchors.fill: parent
                    color: containsMouse ? Qt.lighter (app.breakColor, 1.2) : app.breakColor
                    fromAngle: 6 * (app.positionOffset + app.pomodoroLength)
                    toAngle:   6 * (app.positionOffset + app.pomodoroLength + app.breakLength)

                    onClicked: button.clicked (event)
                    onMoved:
                    {
                        let time = (angle / 6) - app.pomodoroLength
                        if ((time < 1) || (time + app.pomodoroLength > 59)) return

                        app.breakLength = time
                        textLabel.text = app.breakLength + "min"
                    }
                    onReleased: textLabel.text = "start"
                }
            }

            Rectangle
            {
                id: buttonFront
                anchors.fill: button
                anchors.margins: app.sizeRate * 0.03
                color: app.mainColor
                radius: button.height / 2

                border.color: mainBgColor
                border.width: app.sizeRate * 0.03
            }

            C.PieSlice
            {
                id: slicePieCover
                anchors.fill: parent
                color: "#0c000000"
                visible: !(app.state === "")
                fromAngle: 0
                toAngle: 359.999
                onClicked: button.clicked (event)
            }

            C.PieSlice
            {
                id: sliceElapsedTime
                anchors.fill: parent
                color: "#1f000000"
                fromAngle: (6 * app.elapsedTimeOffset) - 0.05
                toAngle:   6 * app.elapsedTimeOffset
                onClicked: button.clicked (event)
            }

            Text
            {
                id: textLabel
                anchors.centerIn: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pointSize: 16
                color: "#f8f9fa"
            }
        }
    }

    Timer
    {
        id: timer
        interval: 1000
        repeat: true
        triggeredOnStart: true

        onTriggered:
        {
            let now = new Date()
            let deltaTime = new Date (now.getTime() - app.lastStartTime.getTime())
            sliceElapsedTime.toAngle = 0.1 * ((app.elapsedTimeOffset * 60) + deltaTime.getSeconds())

            if (now < nextEndTime) return

            app.lastStartTime = new Date (now.getTime())

            if (app.state === "running")
            {
                app.nextEndTime = new Date (now.getTime() + (breakLength * 60000))

                app.elapsedTimeOffset = app.positionOffset + app.pomodoroLength

                app.state = "break"
                messageDialog.text = qsTr ("Pomodoro has finished!")
            }
            else
            {
                app.nextEndTime = new Date (now.getTime() + (pomodoroLength * 60000))

                app.positionOffset    = sliceBreakLength.toAngle / 6
                app.elapsedTimeOffset = sliceBreakLength.toAngle / 6

                app.state = "running"
                messageDialog.text = qsTr ("Break has finished!")
            }

            sliceElapsedTime.toAngle = 0.1 * ((app.elapsedTimeOffset * 60) + deltaTime.getSeconds())

            messageDialog.visible = true
            sound.play()
        }
    }

    SoundEffect
    {
        id: sound

        source: "RingTone.wav"
    }


    MessageDialog
    {
        id: messageDialog
    }
}
