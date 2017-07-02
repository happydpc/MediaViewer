import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import MediaViewer 0.1


//
// Media view
//
Rectangle {
	id: root

	// always fill parent
	anchors.fill: parent

	// externally set
	property var selection
	property var stateManager

	// default mouse handling
	MouseArea {
		anchors.fill: parent
		acceptedButtons: Qt.LeftButton

		// double click: toggle fullscreen state
		onDoubleClicked: if (selection.currentMedia) { stateManager.toggleFullScreen(); }
	}

	// default key handling
	Keys.onPressed: {
		switch (event.key) {
			// previous media
			case Qt.Key_Left:
			case Qt.Key_Up:
				event.accepted = true;
				selection.selectPrevious();
				break;

			// next media
			case Qt.Key_Right:
			case Qt.Key_Down:
				event.accepted = true;
				selection.selectNext();
				break;

			// enter toggles full screen
			case Qt.Key_Return:
			case Qt.Key_Enter:
				event.accepted = true;
				if (selection.currentMedia) {
					stateManager.toggleFullScreen();
				}
				break;

			// escape goes back to preview
			case Qt.Key_Escape:
				event.accepted = true;
				stateManager.state = "preview";
				break;

			default:
				event.accepted = false;
		}
	}

	// detect media changes
	Connections {
		target: selection
		onCurrentMediaChanged: {
			if (selection.currentMedia) {
				switch (selection.currentMedia.type) {
					case Media.Image:
						viewer.source = "qrc:///Viewers/Image.qml";
						viewer.item.source = "file:///" + selection.currentMedia.path;
						break;

					case Media.AnimatedImage:
						viewer.source = "qrc:///Viewers/AnimatedImage.qml";
						viewer.item.source = "file:///" + selection.currentMedia.path;
						break;

					case Media.Movie:
						viewer.source = "qrc:///Viewers/Movie.qml";
						viewer.item.source = "file:///" + selection.currentMedia.path;
						break;

					case Media.NotSupported:
						viewer.source = undefined;
						break;
				}
			} else {
				viewer.source = "";
			}
		}
	}

	// set the focus to the correct item, depending on the various states
	// of the viewer.
	function updateFocus() {
		// do nothing when no selection
		if (selection.currentMedia) {
			var isFullScreen = stateManager.state === "fullscreen";

			// if we have a movie, give focus to the inner loader
			if (selection.currentMedia.type === Media.Movie) {
				root.focus = isFullScreen;
				viewer.focus = isFullScreen;
			} else {
				viewer.focus = false;
				root.focus = isFullScreen;
			}
		}
	}

	// handle focus for the movie viewer
	Connections { target: stateManager; onStateChanged: updateFocus() }
	Connections { target: selection; onCurrentMediaChanged: updateFocus() }

	// the content
	Loader {
		id: viewer
		asynchronous: false
		anchors.fill: parent

		// make selection and state manager available
		property var selection: root.selection
		property var stateManager: root.stateManager
	}
}
