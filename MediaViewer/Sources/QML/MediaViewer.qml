import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtMultimedia 5.6


//
// Media view
//
Rectangle {
	id: root

	// externally set
	property var selection
	property var stateManager

	//
	// fullscreen / preview states
	//
	state: stateManager ? stateManager.state : "preview"
	states: [
		State {
			name: "fullscreen"
			ParentChange { target: image; parent: fullScreenItem }
			PropertyChanges { target: image; focus: true }
		},
		State {
			name: "preview"
			ParentChange { target: image; parent: root }
			PropertyChanges { target: image; focus: false }
		}
	]

	//
	// The window that's used to display the image in fullscreen
	//
	Window {
		id: fullScreenWindow

		// configure
		color: root.color
		flags: Qt.SplashScreen
		visibility: root.state == "fullscreen" ? Window.FullScreen : Window.Hidden
		width: Screen.width
		height: Screen.height

		// quit the main application when the user closes the window in fullscreen mode
		// (by default, it will only close the fullscreen window, leaving the application
		// in a broken state)
		onClosing: {
			close.accepted = true;
			Qt.quit();
		}

		//
		// The content item (we can't use the window as a parent in a
		// ParentChange state change)
		//
		Rectangle {
			id: fullScreenItem
			color: fullScreenWindow.color
			anchors.fill: parent
		}
	}

	//
	// The movie
	//

	//
	// The image (animated or static)
	//
	AnimatedImage {
		id: image
		anchors.fill: parent

		onActiveFocusChanged: if (activeFocus == false) { stateManager.state = "preview"; }
		onStatusChanged: playing = (status == AnimatedImage.Ready)

		// bind the source
		source: selection ? selection.currentImagePath : "qrc:///images/empty"

		// only fit when the image is greater than the view size
		fillMode: sourceSize.width > width || sourceSize.height > height ? Image.PreserveAspectFit : Image.Pad

		// configure the image
		asynchronous: true
		antialiasing: true
		autoTransform: true
		smooth: true
		mipmap: true

		//
		// Mouse area to catch double clicks
		//
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

		//
		// keyboard handling
		//
		Keys.onPressed: {
			if (activeFocus == false) {
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
	}
}
