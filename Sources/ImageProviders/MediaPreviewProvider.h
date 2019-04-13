#ifndef IMAGE_PROVIDERS_MEDIA_PREVIEW_PROVIDER_H
#define IMAGE_PROVIDERS_MEDIA_PREVIEW_PROVIDER_H


namespace MediaViewer
{

	//!
	//! Custom image provider used to generate previews of images, optionally handling caching
	//!
	class MediaPreviewProvider
		: public QObject
		, public QQuickAsyncImageProvider
	{

		Q_OBJECT

		Q_PROPERTY(bool useCache READ GetUseCache WRITE SetUseCache NOTIFY useCacheChanged)
		Q_PROPERTY(QString cachePath READ GetCachePath WRITE SetCachePath NOTIFY cachePathChanged)

	signals:

		void	useCacheChanged(bool useCache);
		void	cachePathChanged(QString cachePath);

	public:

		MediaPreviewProvider(void);

		// reimplemented from QQuickAsyncImageProvider
		QQuickImageResponse *requestImageResponse(const QString &id, const QSize &requestedSize) final;

		// public C++ API
		bool				GetUseCache(void) const;
		void				SetUseCache(bool value);
		const QString &		GetCachePath(void) const;
		void				SetCachePath(const QString & path);

		// public QML API
		Q_INVOKABLE void	clearCache(void) const;

	private:

		// private API
		QString	GetCacheFolder(uint32_t hash) const;
		QImage	GetImagePreview(const QString & path, int width, int height);
		QImage	GetMoviePreview(const QString & path, int width, int height);

		//! true if we should cache the thumbnails, false otherwise
		bool m_UseCache;

		//! path to the thumbnail cache
		QString m_CachePath;

	};


	//!
	//! Utility class used with a QMediaPlayer to capture a frame of a movie.
	//!
	class VideoCapture
		: public QAbstractVideoSurface
	{

	public:

		// Constructor
		VideoCapture(const QString & path, QEventLoop & loop);

		// API
		void			Capture(int retries);
		const QImage &	GetFrame(void) const;

	protected:

		// Reimplemented from QAbstractVideoSurface
		bool								present(const QVideoFrame & source) override;
		QList< QVideoFrame::PixelFormat >	supportedPixelFormats(QAbstractVideoBuffer::HandleType type) const override;

	private:

		//! The movie path (for debugging only)
		QString m_Path;

		//! Reference to the event loop used to wait until the capture's done
		QEventLoop & m_Loop;

		//! When true, the next presented frame will be captured
		bool m_Capture;

		//! The captured frame
		QImage m_Frame;

		//! Number of times to retry capturing
		int m_Retries;

	};


} // namespace MediaViewer


#endif // IMAGE_PROVIDERS_MEDIA_PREVIEW_PROVIDER_H
