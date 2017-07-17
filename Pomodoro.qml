import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Dialogs 1.1
import io.github.witte 1.0 as C
/*
    THINGS TO HAVE:
        - settings
            !- custom pomodoro/break lengths
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
    title: qsTr("Pomodoro")
    color: mainBgColor

    property double sizeRate: (height > width) ? (app.width * 0.75) : (app.height * 0.75)
    property string state: ""
    property string mainColor: stoppedColor
    property string mainBgColor: "#eff0f1"
    property string stoppedColor: "#5d6176"
    property string pomodoroColor: "#aa0000"
    property string breakColor: "#336632"
    property int pomodoroLength: Math.round(slicePomodoroLength.angleSpan / 6) * 60 // pomodoro time in seconds
    property int breakLength: Math.round(sliceBreakLength.angleSpan / 6) * 60 // break time in seconds
    property int elapsedTime: 0 // elapsedTime in seconds
    property double dialTimeUnit: 0

    Item
    {
        states:
        [
            State
            {
                when: (app.state === "") //stopped
                PropertyChanges { target: app; mainColor: app.stoppedColor }
                PropertyChanges { target: textLabel; text: qsTr("start") }
            },
            State
            {
                when: (app.state === "running")
                PropertyChanges { target: app; mainColor: app.pomodoroColor }
                PropertyChanges { target: textLabel; text: qsTr("stop") }
            },
            State
            {
                when: (app.state === "break")
                PropertyChanges { target: app; mainColor: app.breakColor  }
                PropertyChanges { target: textLabel; text: qsTr("stop") }
            }
        ]
    }

    Item
    {
        id: itemRef
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        width: app.sizeRate
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
            width: app.sizeRate * 0.95
            height: width
            onClicked:
            {
                if (app.state !== "")
                {
                    sliceElapsedTime.angleSpan = 0
                    app.elapsedTime = 0
                    app.state = ""
                    timer.stop()
                }
                else
                {
                    app.elapsedTime = 0
                    sliceElapsedTime.angleSpan = 0

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
                    fromAngle: 0
                    angleSpan: 150
                    color: containsMouse ? Qt.lighter(baseColor, 1.2) : baseColor

                    onClicked: button.clicked(event)
                    onMoved: lengthsPie.handleSlicesMove(slicePomodoroLength, deltaAngle)
                    onReleased: textLabel.text = "start"
                }
                C.PieSlice
                {
                    id: sliceBreakLength
                    anchors.fill: parent
                    color: containsMouse ? Qt.lighter(app.breakColor, 1.2) : app.breakColor
                    fromAngle: slicePomodoroLength.angleSpan-0.5
                    angleSpan: 30

                    onClicked: button.clicked(event)
                    onMoved: lengthsPie.handleSlicesMove(sliceBreakLength, deltaAngle)
                    onReleased: textLabel.text = "start"
                }

                function handleSlicesMove(sourceElement, deltaAngle)
                {
                    if (deltaAngle > 0)
                    {
                        var i = (slicePomodoroLength.angleSpan + sliceBreakLength.angleSpan)
                        if ( (i + deltaAngle) < 360)
                        {
                            sourceElement.angleSpan += deltaAngle
                        }
                        else
                            sourceElement.angleSpan += (360 - i)
                    }
                    else
                    {
                        var a = sourceElement.angleSpan + deltaAngle
                        if (a > 5) sourceElement.angleSpan += deltaAngle
                    }
                    textLabel.text = Math.round(sourceElement.angleSpan / 6) + "min"
                }
            }

            Rectangle
            {
                id: buttonFront
                anchors.fill: button
                anchors.margins: app.sizeRate * 0.03
                color: app.mainColor
                radius: button.height/2

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
                angleSpan: 359.999
                onClicked: button.clicked(event)
            }
            C.PieSlice
            {
                id: sliceElapsedTime
                anchors.fill: parent
                color: "#1f000000"
                fromAngle: 0
                angleSpan: 0
                onClicked: button.clicked(event)
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
        onTriggered:
        {
            app.elapsedTime += 1
            sliceElapsedTime.angleSpan += 0.1 // 360 degrees / 60 minutes / 60 seconds

            if (app.state === "running")
            {
                if (app.elapsedTime >= app.pomodoroLength)
                {
                    messageDialog.text = qsTr("Pomodoro has finished!")
                    app.elapsedTime = 0
                    app.state = "break"
                    messageDialog.visible = true
                }
            }
            else
            {
                if (app.elapsedTime >= app.breakLength)
                {
                    messageDialog.text = qsTr("Break has finished!")
                    app.elapsedTime = 0
                    app.state = "running"
                    messageDialog.visible = true
                    sliceElapsedTime.angleSpan = 0
                }
            }
        }
    }

    MessageDialog
    {
        id: messageDialog
    }
}
