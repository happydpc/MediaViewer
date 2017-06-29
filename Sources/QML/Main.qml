import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Controls 2.2 as Controls
import QtQuick.Layouts 1.0
import Qt.labs.settings 1.0
import MediaViewer 0.1


//
// The main window
//
MainWindow {
	id: mainWindow

	// initialize starting folders
	Component.onCompleted: {
		if (initFolder !== "") {
			folderBrowser.currentFolderPath = initFolder;
		}
		if (initMedia !== "") {
			mediaSelection.selectByPath(initMedia);
			stateManager.state = "fullscreen";
		}
	}

	// default settings
	Settings {
		id: settings

		// size of the viewer part
		property alias mediaViewerWidth: mediaViewerContainer.width
		property alias mediaViewerHeight: mediaViewerContainer.height
	}

	// the folder model. Allow access to the physical folders.
	FolderModel {
		id: folderModel
		rootPaths: drives
	}

	// the media  model. Allow access to the medias in the current folder.
	MediaModel {
		id: mediaModel
		root: folderBrowser.currentFolderPath
	}

	// global media selection (needed to share selection between the
	// media browser and the media preview)
	MediaSelection {
		id: mediaSelection
		model: mediaModel
	}

	// connect the media selection and the folder browser
	Connections {
		target: folderBrowser
		onCurrentFolderPathChanged: mediaSelection.clear()
	}

	// global state manager. Used to control fullscren/browsing mode
	StateManager {
		id: stateManager
		mediaBrowser: mediaBrowser
		mediaViewer: mediaViewer
		mainWindow: mainWindow
	}

	// the preferences dialog
	Preferences {
		id: preferences
		settings: Settings {
			category: "Preferences"
			property int playAnimatedImages: 2
			property int playMovies: 2
			property int sortBy: 4
			property int sortOrder: 0
			property double thumbnailSize: 0.5
		}
	}

	// Menu
	header: MainMenu {
	}

	// The split between the media preview and folder browser on the left,
	// and the media browser on the right
	SplitView {
		anchors.fill: parent
		orientation: Qt.Horizontal

		// split between the folders and the media preview
		SplitView {
			orientation: Qt.Vertical

			// folder view
			FolderBrowser {
				id: folderBrowser
				Layout.fillHeight: true
				model: folderModel
			}

			// media preview
			Item {
				id: mediaViewerContainer
				width: 300
				height: 300

				MediaViewer {
					id: mediaViewer
					color: Qt.rgba(0, 0, 0, 1);
					selection: mediaSelection
					stateManager: stateManager
				}
			}
		}

		// media browser
		MediaBrowser {
			id: mediaBrowser
			Layout.fillWidth: true
			selection: mediaSelection
			stateManager: stateManager
			settings: preferences.settings
		}
	}
}
