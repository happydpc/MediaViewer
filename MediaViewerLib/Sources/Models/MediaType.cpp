#include "MediaViewerLibPCH.h"
#include "MediaType.h"


namespace MediaViewerLib
{

	//!
	//! Private list of supported media extension, and their associated MediaType
	//!
	QHash< QString, MediaType > SupportedMediaExtensions = {
		// static images
		{ "jpg",	MediaType::Image },
		{ "jpeg",	MediaType::Image },
		{ "tif",	MediaType::Image },
		{ "tiff",	MediaType::Image },
		{ "png",	MediaType::Image },
		{ "dds",	MediaType::Image },
		{ "svg",	MediaType::Image },

		// animated images
		{ "gif",	MediaType::AnimatedImage },

		// movies
		{ "mp4",	MediaType::Movie },
		{ "mov",	MediaType::Movie }
	};

	//!
	//! Checks if a file is a supported media
	//!
	bool IsMedia(const QString & filename)
	{
		return GetMediaType(filename) != MediaType::NotSupported;
	}

	//!
	//! Get the type of a file
	//!
	MediaType GetMediaType(const QString & filename)
	{
		QString extension = filename.section('.', -1, -1).toLower();
		auto type = SupportedMediaExtensions.find(extension);
		return type != SupportedMediaExtensions.end() ? type.value() : MediaType::NotSupported;
	}

} // namespace MediaViewerLib
