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
		~MediaPreviewProvider(void);

		// reimplemented from QQuickAsyncImageProvider
		QQuickImageResponse *requestImageResponse(const QString &id, const QSize &requestedSize) final;

		// public C++ API
		static QString		DefaultCachePath(void);
		bool				GetUseCache(void) const;
		void				SetUseCache(bool value);
		const QString &		GetCachePath(void) const;
		void				SetCachePath(const QString & path);

		// public QML API
		Q_INVOKABLE void	clearCache(void) const;
		Q_INVOKABLE void	cancelPending(void);

	private:

		// private API
		QString	GetCacheFolder(uint32_t hash) const;
		QImage	GetImagePreview(const QString & path, int width, int height, std::atomic_bool & cancel);
		QImage	GetMoviePreview(const QString & path, int width, int height, std::atomic_bool & cancel);

		//! true if we should cache the thumbnails, false otherwise
		bool m_UseCache;

		//! path to the thumbnail cache
		QString m_CachePath;

		//! pool used to handle the image responses
		QThreadPool m_Pool;

		//! time of the last call to cancelPending
		QTime m_CancelTime;


	};


	//!
	//! Utility class used with a QMediaPlayer to capture a frame of a movie.
	//!
	class VideoCapture
		: public QAbstractVideoSurface
	{

	public:

		// Constructor
		VideoCapture(const QString & path, QEventLoop & loop, std::atomic_bool & cancel);

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

		//! Used to stop the capture in case of errors
		QEventLoop & m_Loop;

		//! Used to cancel the capture
		std::atomic_bool & m_Cancel;

		//! When true, the next presented frame will be captured
		std::atomic_bool m_Capture;

		//! Number of times to retry capturing
		std::atomic_int m_Retries;

		//! The captured frame
		QImage m_Frame;

	};


}


#endif
