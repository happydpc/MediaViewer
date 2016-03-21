import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import QtQml.Models 2.2


//
// The images browser. We use a grid view which is a flickable view, so We
// need to add a scroll view to make the scroll bar visible on Desktop
//
ScrollView {
	id: scrollView

	// alias
	property alias folderPath: root.folderPath
	property alias model: root.model

	// the selection model (used to sync the browser and the viewer)
	property var selection

	// the grid view
	GridView {
		id: root

		// the root folder
		property string folderPath: ""
		onFolderPathChanged: {
			if (model) {
				model.root = folderPath;
				selection.selectByPath(folderPath);
			}
		}

		// update on selection change
		onCurrentItemChanged: {
			if (currentItem) {
				selection.selectByPath(currentItem.currentPath);
			} else {
				selection.selectByPath("");
			}
		}

		// size of the cells
		cellWidth: 220
		cellHeight: 230

		// disable animation of the selection
		highlightFollowsCurrentItem: false

		// delegates
		delegate: itemDelegate
		highlight: highlightDelegate

		// delegate used to draw an item
		Component {
			id: itemDelegate
			Item {
				property string currentPath: path
				width: root.cellWidth
				height: root.cellHeight
				Item {
					width: image.width
					height: image.height + label.height
					anchors.centerIn: parent
					Image {
						id: image
						source: "file:///" + path
						sourceSize.width: root.cellWidth - 20
						sourceSize.height: root.cellHeight - 20 - (label.visible ? label.height + 20 : 0)
						anchors.horizontalCenter: parent.horizontalCenter
						fillMode: sourceSize.width > width || sourceSize.height > height ? Image.PreserveAspectFit : Image.Pad
						asynchronous: true
						antialiasing: true
						autoTransform: true
						smooth: true
						mipmap: true
					}
					Text {
						id: label
						width: root.cellWidth - 20
						horizontalAlignment: Text.AlignHCenter
						elide: Text.ElideRight
						text: name
						anchors.top: image.bottom
						anchors.topMargin: 10
						anchors.horizontalCenter: parent.horizontalCenter
					}
				}
			}
		}

		// delegate used to draw the selected highlight
		Component {
			id: highlightDelegate
			Rectangle {
				x: root.currentItem ? root.currentItem.x : 0
				y: root.currentItem ? root.currentItem.y : 0
				width: root.cellWidth
				height: root.cellHeight
				color: "lightBlue"
			}
		}

		// Mouse handling
		MouseArea {
			anchors.fill: parent
			acceptedButtons: Qt.LeftButton
			onClicked: {
				// get the focus on click
				scrollView.focus = true;

				// update the current index
				root.currentIndex = root.indexAt(
					mouse.x + root.contentX,
					mouse.y + root.contentY
				);
			}
		}
	}

	// keyboard handling
	Keys.onPressed: {
		switch (event.key) {
			case Qt.Key_Down:
				event.accepted = true;
				root.moveCurrentIndexDown();
				break;
			case Qt.Key_Up:
				event.accepted = true;
				root.moveCurrentIndexUp();
				break;
			case Qt.Key_Left:
				event.accepted = true;
				root.moveCurrentIndexLeft();
				break;
			case Qt.Key_Right:
				event.accepted = true;
				root.moveCurrentIndexRight();
				break;
		}
	}
}
