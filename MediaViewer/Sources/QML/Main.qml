import QtQuick 2.5
import QtQuick.Window 2.2
import QtQml.Models 2.2


//
// ImageBrowserer library
//
import MediaViewerLib 0.1


//
// The main window
//
Window {
	id: mainWindow

	visible: true

	//
	// Font
	//
	FontLoader {
		id: sourceSans;
		property int size: 12		
		source: "qrc:///fonts/SourceSansPro-Regular"
	}

	//
	// Save the window's settings
	//
	WindowSettings {
		category: "MainWindow"
		window: mainWindow
		width: 1000
		height: 750
	}

	//
	// The models
	//
	FolderModel { id: __folderModel; rootPaths: drives }
	MediaModel { id: __mediaModel }
	MediaSelection { id: __mediaSelection; model: __mediaModel }

	//
	// 'Constructor'
	//
	Component.onCompleted: {
		console.log("reloaded");
	}	

	//
	// Set fullscreen mode
	//
	property bool fullscreen: false
	property var previousVisibility
	function setFullscreen(value) {
		// checks for state changes
		if (value === fullscreen) {
			return;
		}

		// update the state
		fullscreen = value;

		// configure the UI
		if (fullscreen === true) {
			loader.sourceComponent = viewerComponent;
			previousVisibility = mainWindow.visibility;
			mainWindow.visibility = Window.FullScreen;
		} else {
			loader.sourceComponent = browserComponent;
			mainWindow.visibility = previousVisibility;
		}
	}

	//
	// The main content, which is a loader to be able to switch between browsing
	// and fullscreen
	//
	Loader {
		id: loader
		anchors.fill: parent
		sourceComponent: browserComponent
		focus: true
	}

	//
	// Browser component
	//
	Component {
		id: browserComponent
		Browser {
			id: browser
			focus: true

			// bind the models
			folderModel: __folderModel
			mediaModel: __mediaModel
			mediaSelection: __mediaSelection

			// Handles keys that were not used by the browser
			Keys.onReturnPressed: {
				event.accepted = true;
				mainWindow.setFullscreen(true);
			}
		}
	}

	//
	// Viewer component
	//
	Component {
		id: viewerComponent
		Viewer {
			id: viewer
			focus: true

			// bind
			model: __mediaModel
			selection: __mediaSelection

			// Handles keys that were not used by the viewer
			Keys.onPressed: {
				switch (event.key) {
					case Qt.Key_Escape:
						event.accepted = true;
						mainWindow.setFullscreen(false);
						break;
				}
			}
			Keys.onReturnPressed: {
				event.accepted = true;
				mainWindow.setFullscreen(false);
			}
		}
	}
}
