import QtQuick 2.5
import QtQml.Models 2.2
import MediaViewerLib 0.1


//
// Image selection model
//
ItemSelectionModel {

	// the current media
	property var currentImage: null
	property var currentImagePath: "qrc:///images/empty"
	property var currentImageType: Media.NotSupported
	property var currentImageIndex: -1

	// set the current media by path
	function selectByPath(path) {
		if (model) {
			var index = model.getModelIndexByPath(path);
			setCurrentIndex(index, ItemSelectionModel.Current);
		}
	}

	// select the previous media
	function selectPrevious() {
		if (model) {
			var index = model.getPreviousModelIndex(currentIndex);
			if (index.valid) {
				setCurrentIndex(index, ItemSelectionModel.Current);
			}
		}
	}

	// select the next media
	function selectNext() {
		if (model) {
			var index = model.getNextModelIndex(currentIndex);
			if (index.valid) {
				setCurrentIndex(index, ItemSelectionModel.Current);
			}
		}
	}

	// detect changes
	onCurrentChanged: {
		if (model) {
			if (current.valid) {
				currentImage = model.getMedia(current);
				currentImagePath = "file:///" + currentImage.path;
				currentImageIndex = model.getIndex(current);
				currentImageType = currentImage.type;
			} else {
				currentImage = null;
				currentImagePath = "qrc:///images/empty";
				currentImageIndex = -1;
				currentImageType = Media.NotSupported;
			}
		}
	}
}
