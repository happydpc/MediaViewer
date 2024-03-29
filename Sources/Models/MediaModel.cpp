#include "MediaViewerPCH.h"
#include "MediaModel.h"
#include "Media.h"


namespace MediaViewer
{

	//!
	//! Constructor.
	//!
	MediaModel::MediaModel(QObject * parent)
		: QAbstractItemModel(parent)
		, m_Dirty(false)
		, m_SortBy(SortBy::None)
		, m_SortOrder(SortOrder::Ascending)
	{
		// setup file watching
		QObject::connect(&m_FileWatcher, &QFileSystemWatcher::directoryChanged, this, &MediaModel::UpdateMedias);
	}

	//!
	//! Destructor.
	//!
	MediaModel::~MediaModel(void)
	{
		this->Clear();
	}

	//!
	//! Set the root's path.
	//!
	void MediaModel::SetRoot(const QString & path)
	{
		if (path == m_Root || QDir(path).exists() == false)
		{
			return;
		}

		// remove the old path from the file watcher
		if (m_Root.isEmpty() == false)
		{
			m_FileWatcher.removePath(m_Root);
		}

		// reset the model
		this->beginResetModel();
		this->Clear();
		m_Root = path;
		this->endResetModel();

		// add the new path to the file watcher
		m_FileWatcher.addPath(m_Root);

		// notify
		emit rootChanged(m_Root);
	}

	//!
	//! Clear the medias
	//!
	void MediaModel::Clear(void)
	{
		for (Media * media : m_Medias)
		{
			MT_DELETE media;
		}
		m_Medias.clear();
		m_Dirty = true;
	}

	//!
	//! Update the medias after a folder change
	//!
	void MediaModel::UpdateMedias(const QString & folder)
	{
		Q_UNUSED(folder);

		// rescan the folder
		QDir root(m_Root);
		QVector< QString > medias;
		for (const auto & file : root.entryList(QDir::Files, QDir::NoSort))
		{
			if (Media::IsMedia(file) == true)
			{
				medias.push_back(file);
			}
		}

		// check deleted images
		int index = 0;
		QVector< int > toRemove;
		for (const Media * media : m_Medias)
		{
			if (medias.contains(media->GetName()) == false)
			{
				toRemove.push_back(index);
			}
			++index;
		}

		// check added images
		QVector< QString > toAdd;
		for (const QString & file : medias)
		{
			bool found = false;
			for (const Media * media : m_Medias)
			{
				if (media->GetName() == file)
				{
					found = true;
					break;
				}
			}
			if (found == false)
			{
				toAdd.push_back(file);
			}
		}

		// remove
		while (toRemove.isEmpty() == false)
		{
			index = toRemove.back();
			this->beginRemoveRows(QModelIndex(), index, index);
			MT_DELETE m_Medias[index];
			m_Medias.remove(index);
			this->endRemoveRows();
			toRemove.pop_back();
		}

		// add
		auto sort = this->GetSortOperator();
		for (const QString & file : toAdd)
		{
			Media * media = MT_NEW Media(root.absoluteFilePath(file));
			index = 0;
			for (Media * m : m_Medias)
			{
				bool res = sort(media, m);
				if (res == false)
				{
					++index;
				}
				else
				{
					this->beginInsertRows(QModelIndex(), index, index);
					m_Medias.insert(index, media);
					this->endInsertRows();
					index = -1;
					break;
				}
			}

			// if we reach here, push back
			if (index != -1)
			{
				this->beginInsertRows(QModelIndex(), index, index);
				m_Medias.push_back(media);
				this->endInsertRows();
			}
		}
	}

	//!
	//! Get the medias in the root folder
	//!
	const QVector< Media * > &	MediaModel::GetMedias(void) const
	{
		if (m_Dirty == true)
		{
			QDir dir(m_Root);
			for (const auto & file : dir.entryList(QDir::Files, QDir::NoSort))
			{
				if (Media::IsMedia(file) == true)
				{
					m_Medias.push_back(MT_NEW Media(dir.absoluteFilePath(file)));
					QQmlEngine::setObjectOwnership(m_Medias.back(), QQmlEngine::CppOwnership);
				}
			}

			// sort
			this->Sort();

			// reset the dirty flag
			m_Dirty = false;
		}
		return m_Medias;
	}

	//!
	//! Set the sort type
	//!
	void MediaModel::SetSortBy(SortBy by)
	{
		if (m_SortBy != by)
		{
			this->sort(by, m_SortOrder);
			emit sortByChanged(by);
		}
	}

	//!
	//! Set the sort direction
	//!
	void MediaModel::SetSortOrder(SortOrder order)
	{
		if (m_SortOrder != order)
		{
			this->sort(m_SortBy, order);
			emit sortOrderChanged(order);
		}
	}

#define SORT_FUNCTOR	\
	std::function< bool (const Media *, const Media *) >([](const Media * l, const Media * r) -> bool

	//!
	//! Get the sort operator
	//!
	std::function< bool (const Media *, const Media *) > MediaModel::GetSortOperator(void) const
	{
		switch (m_SortBy)
		{
			case SortBy::Name:
				return m_SortOrder == SortOrder::Ascending ?
					SORT_FUNCTOR { return l->GetName() < r->GetName(); }) :
					SORT_FUNCTOR { return l->GetName() > r->GetName(); });

			case SortBy::Date:
				return m_SortOrder == SortOrder::Ascending ?
					SORT_FUNCTOR { return l->GetDate() < r->GetDate(); }) :
					SORT_FUNCTOR { return l->GetDate() > r->GetDate(); });

			case SortBy::Size:
				return m_SortOrder == SortOrder::Ascending ?
					SORT_FUNCTOR { return l->GetSize() < r->GetSize(); }) :
					SORT_FUNCTOR { return l->GetSize() > r->GetSize(); });

			case SortBy::Type:
				return m_SortOrder == SortOrder::Ascending ?
					SORT_FUNCTOR { return l->GetType() < r->GetType(); }) :
					SORT_FUNCTOR { return l->GetType() > r->GetType(); });

			default:
				return SORT_FUNCTOR {
					Q_UNUSED(l);
					Q_UNUSED(r);
					return false;
				});
		}
	}

#undef SORT_FUNCTOR

	//!
	//! Sort the model
	//!
	void MediaModel::Sort(void) const
	{
		::Sort(m_Medias, this->GetSortOperator());
	}

	//!
	//! The QML invokable sort method
	//!
	void MediaModel::sort(SortBy by, SortOrder order)
	{
		bool needSort = false;
		if (m_SortBy != by)
		{
			needSort = true;
			m_SortBy = by;
			emit sortByChanged(by);
		}
		if (m_SortOrder != order)
		{
			needSort = true;
			m_SortOrder = order;
			emit sortOrderChanged(order);
		}

		if (needSort == true)
		{
			this->beginResetModel();
			this->Sort();
			this->endResetModel();
		}
	}

	//!
	//! Get the index from a path
	//!
	int MediaModel::getIndexByPath(const QString & path) const
	{
		int index = 0;
		for (const Media * media : this->GetMedias())
		{
			if (media->GetPath() == path)
			{
				return index;
			}
			++index;
		}
		return -1;
	}

	//!
	//! Get the model index from a path
	//!
	QModelIndex MediaModel::getModelIndexByPath(const QString & path) const
	{
		int index = this->getIndexByPath(path);
		return index != -1 ? this->createIndex(index, 0, const_cast< Media * >(this->GetMedias()[index])) : QModelIndex();
	}

	//!
	//! Get the model previous index
	//!
	QModelIndex MediaModel::getPreviousModelIndex(const QModelIndex & index) const
	{
		Q_ASSERT(index.isValid() == true && index.row() >= 0 && index.row() < this->GetMedias().size());
		int newIndex = qMax(index.row() - 1, 0);
		return this->createIndex(newIndex, 0, this->GetMedias()[newIndex]);
	}

	//!
	//! Get the next model index
	//!
	QModelIndex MediaModel::getNextModelIndex(const QModelIndex & index) const
	{
		Q_ASSERT(index.isValid() == true && index.row() >= 0 && index.row() < this->GetMedias().size());
		int newIndex = qMin(index.row() + 1, this->GetMedias().size() - 1);
		return this->createIndex(newIndex, 0, this->GetMedias()[newIndex]);
	}

	//!
	//! Get a model index from an index
	//!
	QModelIndex MediaModel::getModelIndexByIndex(int index) const
	{
		return (index >= 0 && index < this->GetMedias().size()) ? this->createIndex(index, 0, this->GetMedias()[index]) : QModelIndex();
	}

	//!
	//! Get a media
	//!
	Media * MediaModel::getMedia(const QModelIndex & index) const
	{
		return index.isValid() == true ? static_cast< Media * >(index.internalPointer()) : nullptr;
	}

	//!
	//! Get the index of a media
	//!
	int MediaModel::getIndex(const QModelIndex & index) const
	{
		return index.isValid() == true ? index.row() : -1;
	}

	//!
	//! Get the roles supported by this model
	//!
	QHash< int, QByteArray > MediaModel::roleNames(void) const
	{
		return {
			{ Qt::DisplayRole,	"name" },
			{ Qt::UserRole,		"path" },
			{ Qt::UserRole + 1,	"date" },
			{ Qt::UserRole + 2,	"size" },
			{ Qt::UserRole + 3,	"type" }
		};
	}

	//!
	//! Get the data for a given cell and row.
	//!
	QVariant MediaModel::data(const QModelIndex & index, int role) const
	 {
		if (index.isValid() == false)
		{
			return QVariant();
		}

		Media * media = static_cast< Media * >(index.internalPointer());
		Q_ASSERT(media != nullptr);

		switch (role)
		{
			case Qt::DisplayRole:	return media->GetName();
			case Qt::UserRole:		return media->GetPath();
			case Qt::UserRole + 1:	return media->GetDate();
			case Qt::UserRole + 2:	return static_cast< qulonglong >(media->GetSize());
			case Qt::UserRole + 3:	return static_cast< int >(media->GetType());
			default:				return QVariant();
		}
	}

	//!
	//! Get an index for a row, column and parent.
	//!
	//! @param row
	//!		Parent relative row index.
	//!
	QModelIndex MediaModel::index(int row, int column, const QModelIndex & parent) const
	{
		Q_UNUSED(parent);
		Q_ASSERT(parent.isValid() == false);
		return this->createIndex(row, column, this->GetMedias().at(row));
	}

	//!
	//! Get the parent of a cell.
	//!
	QModelIndex MediaModel::parent(const QModelIndex & /* index */) const
	{
		return QModelIndex();
	}

	//!
	//! Get the number of row (e.g. children) of a cell.
	//!
	int MediaModel::rowCount(const QModelIndex & parent) const
	{
		return parent.isValid() == true ? 0 : this->GetMedias().size();
	}

	//!
	//! Get the number of columns.
	//!
	int MediaModel::columnCount(const QModelIndex & /* parent */) const
	{
		return 1;
	}

} // namespace MediaViewer
