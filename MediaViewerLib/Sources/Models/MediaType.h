#ifndef __UTILS_MEDIA_TYPE_H__
#define	__UTILS_MEDIA_TYPE_H__


namespace MediaViewerLib
{

	//!
	//! The different type of media
	//!
	enum class MediaType
	{
		//! A simple static image
		Image = 0,

		//! An animated image
		AnimatedImage,

		//! A movie
		Movie,

		//! Not a media
		NotSupported,
	};

	// utilities
	bool		IsMedia(const QString & filename);
	MediaType	GetMediaType(const QString & filename);

} // namespace MediaViewerLib


#endif // __UTILS_MEDIA_TYPE_H__
