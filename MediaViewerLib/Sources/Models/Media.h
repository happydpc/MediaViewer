#ifndef __MEDIA_H__
#define __MEDIA_H__


#include "MediaType.h"


namespace MediaViewerLib
{
	class Media;
}


// This is needed to remove the C4251 warning
// See : http://support.microsoft.com/kb/168958/en-us
// Also note that it must be declared before the actual use.
MEDIA_VIEWER_LIB_IMPORT_TEMPLATE template class MEDIA_VIEWER_LIB_EXPORT QVector< MediaViewerLib::Media * >;


namespace MediaViewerLib
{

	//!
	//! This class represents a media file.
	//!
	class MEDIA_VIEWER_LIB_EXPORT Media
		: public QObject
	{

		Q_OBJECT

		Q_ENUMS(MediaType)
		Q_PROPERTY(QString path READ GetPath)
		Q_PROPERTY(QString name READ GetName)
		Q_PROPERTY(MediaType type READ GetType)

	public:

		Media(const QString & path = "");
		Media(const Media & other);
		~Media(void);

		// public API
		const QString &		GetPath(void) const;
		const QString &		GetName(void) const;
		MediaType			GetType(void) const;

	private:

		//! The media path
		QString m_Path;

		//! The media name
		QString m_Name;

		//! The media type
		MediaType m_Type;

	};

} // namespace MediaViewerLib


#endif // __MEDIA_H__
