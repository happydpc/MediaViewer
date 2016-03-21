import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.4


//
// Media view
//
Rectangle {

	// alias
	property alias source: image.source
	property alias asynchronous: image.asynchronous
	property alias antialiasing: image.antialiasing
	property alias autoTransform: image.autoTransform
	property alias smooth: image.smooth
	property alias mipmap: image.mipmap


	//-------------------------------------------------------------------------
	// Privates

	// The image
	Image {
		id: image
		anchors.fill: parent

		// only fit when the image is greater than the view size
		fillMode: sourceSize.width > width || sourceSize.height > height ? Image.PreserveAspectFit : Image.Pad

		// configure the image
		asynchronous: true
		antialiasing: true
		autoTransform: true
		smooth: true
		mipmap: true
	}
}
