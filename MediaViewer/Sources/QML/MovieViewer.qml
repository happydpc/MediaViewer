import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import QtMultimedia 5.6
import MediaViewerLib 0.1


//
// Movie viewer
//
VideoOutput {
	id: root
	source: mediaPlayer

	// externally set
	property var selection
	property var stateManager

	// only enable for movies
	enabled: selection && selection.currentMediaType == Media.Movie
	visible: enabled
	focus: enabled && stateManager.state === "fullscreen"

	// when loosing focus, switch back to preview state
	onActiveFocusChanged: if (activeFocus === false && selection.currentMediaType === Media.Movie) { stateManager.state = "preview"; }

	// playing state
	property bool isPlaying: true

	// the media player
	MediaPlayer {
		id: mediaPlayer
		source: (enabled && selection) ? "file:///" + selection.currentMediaPath : ""
		autoPlay: true
		onError: {
			if (MediaPlayer.NoError != error) {
				console.log("[qmlvideo] VideoItem.onError error " + error + " errorString " + errorString)
			}
		}

		// update playing state
		onPaused: root.isPlaying = false
		onPlaying: root.isPlaying = true
		onStopped: root.isPlaying = false
	}

	// keyboard navigation
	Keys.onPressed: {
		if (activeFocus === false) {
			return;
		}
		switch (event.key) {
			case Qt.Key_Left:
			case Qt.Key_Up:
				event.accepted = true;
				selection.selectPrevious();
				break;
			case Qt.Key_Right:
			case Qt.Key_Down:
				event.accepted = true;
				selection.selectNext();
				break;
			case Qt.Key_Return:
			case Qt.Key_Enter:
			case Qt.Key_Escape:
				stateManager.state = "preview";
				break;
		}
	}

	// Mouse area to catch double clicks
	MouseArea {
		anchors.fill: parent
		acceptedButtons: Qt.LeftButton
		onDoubleClicked: {
			if (stateManager.state == "fullscreen") {
				stateManager.state = "preview";
			} else {
				stateManager.state = "fullscreen";
			}
		}
	}

	// playback controls
	Rectangle {
		anchors {
			bottom: parent.bottom
			bottomMargin: 50
			horizontalCenter: parent.horizontalCenter
		}
		width: childrenRect.width
		height: childrenRect.height
		radius: 10
		color: Qt.rgba(0.3, 0.3, 0.3, 0.3)
		RowLayout {
			Image {
				width: 50
				height: 50
				fillMode: Image.PreserveAspectFit
				source: "qrc:/icons/stop"
				MouseArea {
					anchors.fill: parent
					acceptedButtons: Qt.LeftButton
					onClicked: mediaPlayer.stop()
				}
			}
			Image {
				width: 50
				height: 50
				fillMode: Image.PreserveAspectFit
				source: root.isPlaying ? "qrc:/icons/pause" : "qrc:/icons/play"
				MouseArea {
					anchors.fill: parent
					acceptedButtons: Qt.LeftButton
					onClicked: root.isPlaying ? mediaPlayer.pause() : mediaPlayer.play()
				}
			}
		}
	}
}