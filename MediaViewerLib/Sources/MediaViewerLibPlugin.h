#ifndef __MEDIA_VIEWER_LIB_PLUGIN_H__
#define __MEDIA_VIEWER_LIB_PLUGIN_H__


namespace MediaViewerLib
{


	//!
	//! Pluging class
	//!
	class MediaViewerLibPlugin
	: public QQmlExtensionPlugin
	{

		Q_OBJECT
		Q_PLUGIN_METADATA(IID "MediaViewerLib")

	public:

		void registerTypes(const char * uri);

	};


} // namespace MediaViewerLib


#endif // __MEDIA_VIEWER_LIB_PLUGIN_H__
