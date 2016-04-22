#ifndef __MEDIA_H__
#define __MEDIA_H__


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

		Q_ENUMS(Type)
		Q_PROPERTY(QString path READ GetPath NOTIFY pathChanged)
		Q_PROPERTY(QString name READ GetName NOTIFY nameChanged)
		Q_PROPERTY(Type type READ GetType NOTIFY typeChanged)

	public:

		//!
		//! The different type of media
		//!
		enum class Type
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

	signals:

		void	pathChanged(const QString & path);
		void	nameChanged(const QString & name);
		void	typeChanged(Type type);

	public:

		Media(const QString & path = "");
		Media(const Media & other);
		~Media(void);

		// public API
		const QString &		GetPath(void) const;
		const QString &		GetName(void) const;
		Type				GetType(void) const;

		// utilities
		static bool		IsMedia(const QString & filename);
		static Type		GetType(const QString & filename);

	private:

		//! The media path
		QString m_Path;

		//! The media name
		QString m_Name;

		//! The media type
		Type m_Type;

	};

} // namespace MediaViewerLib


#endif // __MEDIA_H__
