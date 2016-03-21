import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import Qt.labs.settings 1.0


//
// The image browser. It allows browsing the harddrives and selecting folders
// to inspect their content in an image browser.
//
Item {
	id: root

	// to bind
	property var folderModel
	property var mediaModel
	property var mediaSelection


	//-------------------------------------------------------------------------
	// Privates
	//

	// Settings
	Settings {
		id: settings
		category: "browser"
		property int previewWidth: 300
		property int previewHeight: 250
		property string currentFolderPath: ""
	}

	// 'constructor'
	Component.onCompleted: {
		// restore the folder's path
		folders.setCurrentFolderPath(settings.currentFolderPath);
	}

	// 'destructor'
	Component.onDestruction: {
		// save the settings
		settings.previewWidth = preview.width;
		settings.previewHeight = preview.height;
		settings.currentFolderPath = folders.currentFolderPath;
	}

	// The split between the imageView + folder view and the images
	SplitView {
		anchors.fill: parent
		orientation: Qt.Horizontal

		// split between the folders and the image view
		SplitView {
			orientation: Qt.Vertical

			// folder view
			FolderBrowser {
				id: folders
				focus: true
				activeFocusOnTab: true
				Layout.fillHeight: true
				model: folderModel
			}

			// Imageview of the image.
			MediaViewer {
				id: preview
				width: settings.previewWidth
				height: settings.previewHeight
				source: mediaSelection.currentImagePath
			}
		}

		// The image browser
		MediaBrowser {
			id: images
			activeFocusOnTab: true
			Layout.fillWidth: true
			focus: true
			model: mediaModel
			selection: mediaSelection
			folderPath: folders.currentFolderPath
		}
	}
}
