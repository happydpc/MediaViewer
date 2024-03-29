#ifndef MODELS_FOLDER_H
#define MODELS_FOLDER_H


namespace MediaViewer
{

	//!
	//! This class represents a folder.
	//!
	class Folder
		: public QObject
	{

		Q_OBJECT

		Q_PROPERTY(QString path READ GetPath WRITE SetPath NOTIFY pathChanged)
		Q_PROPERTY(QString name READ GetName NOTIFY nameChanged)
		Q_PROPERTY(int mediaCount READ GetMediaCount NOTIFY mediaCountChanged)

	signals:

		void pathChanged(const QString & path) const;
		void nameChanged(const QString & name) const;
		void mediaCountChanged(int mediaCount) const;

	public:

		Folder(const QString & path = "", const Folder * parent = nullptr);
		Folder(const Folder & other);
		~Folder(void);

		// public API
		inline const QString &					GetPath(void) const;
		inline const QString &					GetName(void) const;
		inline int								GetMediaCount(void) const;
		inline const Folder *					GetParent(void) const;
		inline const QVector< Folder * > &		GetChildren(void) const;
		inline static QString					Normalize(const QString & path);

		// QML API
		Q_INVOKABLE void	collapse(void) const;

	private:

		// private API
		void	Clear(void);
		void	UpdateChildren(void) const;
		void	UpdateMedias(void) const;
		void	SetPath(const QString & path);

		//! The parent
		const Folder * m_Parent;

		//! The folder's path
		QString m_Path;

		//! The name of the folder
		QString m_Name;

		//! The number of medias
		mutable int m_MediaCount;

		//! True when the children's list is dirty
		mutable bool m_Dirty;

		//! The children
		mutable QVector< Folder * > m_Children;

		//! File watcher used to detect file changes in the current folder
		QFileSystemWatcher m_FileWatcher;

	};

}


#include "Folder.inl"


#endif
