#ifndef __FOLDER_MODEL_H__
#define __FOLDER_MODEL_H__


namespace MediaViewerLib
{
	class Folder;
}


// This is needed to remove the C4251 warning
// See : http://support.microsoft.com/kb/168958/en-us
// Also note that it must be declared before the actual use.
MEDIA_VIEWER_LIB_IMPORT_TEMPLATE template class MEDIA_VIEWER_LIB_EXPORT QVector< MediaViewerLib::Folder * >;


namespace MediaViewerLib
{

	//!
	//! Model used to represent a folder hierarchy
	//!
	class MEDIA_VIEWER_LIB_EXPORT FolderModel
		: public QAbstractItemModel
	{

		Q_OBJECT

		// note: the namespace is needed here
		Q_PROPERTY(QQmlListProperty< MediaViewerLib::Folder > roots READ GetRoots)
		Q_PROPERTY(QStringList rootPaths READ GetRootPaths WRITE SetRootPaths)

	public:

		FolderModel(QObject * parent = nullptr);
		~FolderModel(void);

		// QAbstractItemModel implementation
		QHash< int, QByteArray >	roleNames(void) const final;
		QVariant					data(const QModelIndex & index, int role = Qt::DisplayRole) const final;
		QModelIndex					index(int row, int column, const QModelIndex & parent = QModelIndex()) const final;
		QModelIndex					parent(const QModelIndex & index) const final;
		int							rowCount(const QModelIndex & parent = QModelIndex()) const final;
		int							columnCount(const QModelIndex & parent = QModelIndex()) const final;

		// public QML API
		Q_INVOKABLE QModelIndex		getIndexByPath(const QString & path) const;

	protected:

		// C++ API
		QQmlListProperty< Folder >	GetRoots(void) const;
		static int					GetRootCount(QQmlListProperty< Folder > * roots);
		static Folder *				GetRoot(QQmlListProperty< Folder > * roots, int index);
		static void					AddRoot(QQmlListProperty< Folder > * roots, Folder * root);
		static void					Clear(QQmlListProperty< Folder > * roots);
		QStringList					GetRootPaths(void) const;
		void						SetRootPaths(const QStringList & paths);

	private:

		//! The root folders
		QVector< Folder * > m_Roots;

	};

} // namespace MediaViewerLib


#endif // __FOLDER_MODEL_H__
