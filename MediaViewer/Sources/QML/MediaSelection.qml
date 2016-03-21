import QtQuick 2.5
import QtQml.Models 2.2


//
// Image selection model
//
ItemSelectionModel {

	// the currently selected image
	property var currentImage

	// the path of the currently selected image
	property var currentImagePath: "qrc:///images/empty"

	// set the current image by path
	function selectByPath(path) {
		if (model) {
			var index = model.getIndexByPath(path);
			setCurrentIndex(index, ItemSelectionModel.Current);
		}
	}

	// select the previous image
	function selectPrevious() {
		if (model) {
			var index = model.getPreviousIndex(currentIndex);
			if (index.valid) {
				setCurrentIndex(index, ItemSelectionModel.Current);
			}
		}
	}

	// select the next image
	function selectNext() {
		if (model) {
			var index = model.getNextIndex(currentIndex);
			if (index.valid) {
				setCurrentIndex(index, ItemSelectionModel.Current);
			}
		}
	}

	// detect changes
	onCurrentChanged: {
		if (current.valid) {
			currentImage = model.getImage(current);
			currentImagePath = "file:///" + currentImage.path;
		} else {
			currentImage = null;
			currentImagePath = "qrc:///images/empty";
		}
	}
}
