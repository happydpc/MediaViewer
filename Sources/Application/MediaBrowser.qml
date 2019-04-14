import QtQuick 2.5
import QtQuick.Controls 1.4 as Controls
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Layouts 1.2
import QtMultimedia 5.8
import MediaViewer 0.1


//
// The media browser.
//
Rectangle {
	id: root

	// externally set
	property var selection
	property var stateManager

	// privates
	property bool _controlDown: false
	property bool _shiftDown: false
	property color _highlight: Material.color(Material.LightBlue, Material.Shade300)
	property color _background: root.color
	property int _thumbnailSize: settings.get("Media.ThumbnailSize")
	property bool _showLabel: settings.get("Media.ShowLabel")

	// bind settings
	Connections {
		target: settings
		onSettingChanged: {
			switch (key) {
				case "Media.SortBy":
					selection.model.sortBy = value;
					break;
					
				case "Media.SortOrder":
					selection.model.sortOrder = value;
					break;

				case "Media.ThumbnailSize":
					root._thumbnailSize = value;
					break;

				case "Media.ShowLabel":
					root._showLabel = value;
					break;
			}
		}
	}

	// handle focus
	function forceFocus() {
		scrollView.forceActiveFocus();
	}

	// use a scroll view to show a scroll bar (GridView is a flickable, so
	// it doesn't show any scroll bar)
	Controls.ScrollView {
		id: scrollView
		anchors.fill: parent

		// the grid view
		GridView {
			id: grid

			// size of the cells
			cellWidth: root._thumbnailSize
			cellHeight: root._thumbnailSize

			// delegates
			delegate: itemDelegate

			// bind the model
			model: selection ? selection.model : undefined

			// delegate used to draw an item
			Component {
				id: itemDelegate

				Rectangle {
					id: thumbnailBackground
					property string currentPath: path
					width: grid.cellWidth
					height: grid.cellHeight
					color: _background

					// check selection change to update the background
					Connections {
						target: selection
						onSelectionChanged: {
							var selected = selection.isSelected(index);
							thumbnailBackground.color = selected ? palette.highlight : _background;
							label.item.color = selected ? palette.highlightedText : palette.windowText;
						}
					}

					// the media preview
					Image {
						id: image

						// size and anchoring
						height: parent.height - 20 - (label.active ? label.height + 10 : 0)
						width: parent.width - 20
						anchors.horizontalCenter: parent.horizontalCenter
						anchors.top: parent.top
						anchors.topMargin: 10

						// generate the preview
						source: "image://MediaPreview/" + path + "?" + width + "&" + height
						asynchronous: true
						fillMode: Image.PreserveAspectFit

						// animation overlay if the media can be played
						Loader {
							active: image.status === Image.Ready && type !== Media.Image

							anchors.centerIn: parent
							width: grid.cellWidth / 2
							height: grid.cellHeight / 2

							sourceComponent: Image {
								source: "qrc:/Icons/Play"
								sourceSize.width: grid.cellWidth / 2
								sourceSize.height: grid.cellHeight / 2
								fillMode: Image.PreserveAspectFit
								opacity: 0.5
							}
						}
					}

					// the label
					Loader {
						id: label
						active: root._showLabel

						// position in the thumbnail
						width: parent.width - 20
						anchors.horizontalCenter: parent.horizontalCenter
						anchors.bottom: parent.bottom
						anchors.bottomMargin: 10

						// the label
						sourceComponent: Label {
							horizontalAlignment: Text.AlignHCenter
							elide: Text.ElideMiddle
							text: name
						}
					}
				}
			}

			// notify the selection manager that the index changed
			onCurrentIndexChanged: {
				if (stateManager.state === "preview") {
					if (_controlDown === true) {
						selection.toggleSelection(grid.currentIndex);
					} else if (_shiftDown === true) {
						selection.extendSelection(grid.currentIndex);
					} else {
						selection.setCurrent(grid.currentIndex);
					}
				}
			}

			// update the view's current item when the selection manager's current index changed
			Connections {
				target: selection
				onCurrentChanged: {
					if (stateManager.state === "fullscreen") {
						grid.currentIndex = selection.current.row;
					}
				}
			}

			// Mouse handling
			MouseArea {
				anchors.fill: parent
				acceptedButtons: Qt.LeftButton | Qt.MiddleButton

				// update selection
				onClicked: {
					// grab the focus (the folder tree view can have it, and after
					// a click on the media, we usually want to browse them with the keyboard)
					// note: don't know why I can't use parent (must use scrollView id) and also
					// don't know why I can't set the focus on grid ...
					scrollView.focus = true;

					// select the item under the mouse
					var index = grid.currentIndex;
					grid.currentIndex = grid.indexAt(
						mouse.x + grid.contentX,
						mouse.y + grid.contentY
					);

					// if the selection is the same, force notification since we might want to
					// toggle the active item's selection
					if (index === grid.currentIndex) {
						grid.currentIndexChanged(index);
					}

					// middle click, toggle fullscreen
					if (selection.currentMedia && mouse.button === Qt.MiddleButton) {
						stateManager.state = "fullscreen";
					}
				}

				// toggle fullscreen
				onDoubleClicked: {
					if (selection.currentMedia) {
						stateManager.state = "fullscreen";
					}
				}
			}
		}

		// Keyboard handling
		Keys.onPressed: {
			switch (event.key) {
				case Qt.Key_Control:
					event.accepted = true;
					_controlDown = true;
					break;
				case Qt.Key_Shift:
					event.accepted = true;
					_shiftDown = true;
					break;
				case Qt.Key_Down:
					event.accepted = true;
					grid.moveCurrentIndexDown();
					break;
				case Qt.Key_Up:
					event.accepted = true;
					grid.moveCurrentIndexUp();
					break;
				case Qt.Key_Left:
					event.accepted = true;
					grid.moveCurrentIndexLeft();
					break;
				case Qt.Key_Right:
					event.accepted = true;
					grid.moveCurrentIndexRight();
					break;
				case Qt.Key_Enter:
				case Qt.Key_Return:
					stateManager.state = "fullscreen";
					break;
			}
		}
		Keys.onReleased: {
			switch (event.key) {
				case Qt.Key_Control:
					event.accepted = true;
					_controlDown = false;
					break;
				case Qt.Key_Shift:
					event.accepted = true;
					_shiftDown = false;
					break;
			}
		}
	}
}
