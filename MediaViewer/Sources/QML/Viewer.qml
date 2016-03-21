import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Window 2.2
import QtQuick.Layouts 1.2
import QtQml.Models 2.2


//
// The fullscreen viewer
//
Rectangle {
	focus: true

	// to bind
	property var model
	property var selection


	//-------------------------------------------------------------------------
	// Privates
	//

	// keyboard handling
	Keys.onPressed: {
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
		}
	}

	// the image
	MediaViewer {
		anchors.fill: parent
		source: selection.currentImagePath
	}
}
