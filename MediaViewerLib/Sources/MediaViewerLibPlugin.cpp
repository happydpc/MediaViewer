#include "MediaViewerLibPCH.h"
#include "MediaViewerLibPlugin.h"
#include "Models/FolderModel.h"
#include "Models/Folder.h"
#include "Models/MediaModel.h"
#include "Models/Media.h"


namespace MediaViewerLib
{

	//!
	//! Register stuff exposed to QML
	//!
	void MediaViewerLibPlugin::registerTypes(const char * uri)
	{
		// check module's name
		Q_UNUSED(uri);
		Q_ASSERT(uri == QString("MediaViewerLib"));
		
		// version of the plugin
		int major = 0;
		int minor = 1;

		// register for use with QVariant and property system
		qRegisterMetaType< Folder * >("Folder*");
		qRegisterMetaType< Media * >("Media*");

		// register our QML types
		qmlRegisterType< Folder >("MediaViewerLib", major, minor, "Folder");
		qmlRegisterType< FolderModel >("MediaViewerLib", major, minor, "FolderModel");
		qmlRegisterType< Media >("MediaViewerLib", major, minor, "Media");
		qmlRegisterType< MediaModel >("MediaViewerLib", major, minor, "MediaModel");
	}

} // namespace MediaViewerLib
