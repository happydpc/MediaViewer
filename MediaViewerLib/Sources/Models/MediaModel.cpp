#include "MediaViewerLibPCH.h"
#include "MediaModel.h"
#include "Media.h"
#include "Utils/Misc.h"


namespace MediaViewerLib
{

	//!
	//! Constructor.
	//!
	MediaModel::MediaModel(QObject * parent)
		: QAbstractItemModel(parent)
		, m_Dirty(false)
	{
	}

	//!
	//! Destructor.
	//!
	MediaModel::~MediaModel(void)
	{
		this->Clear();
	}

	//!
	//! Get the current root's path.
	//!
	const QString & MediaModel::GetRoot(void) const
	{
		return m_Root;
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

		this->beginResetModel();
		this->Clear();

		m_Root = path;
		m_Dirty = true;

		this->endResetModel();

		// notify
		emit rootChanged(m_Root);
	}

	//!
	//! Clear the images
	//!
	void MediaModel::Clear(void)
	{
		for (Media * image : m_Images)
		{
			DELETE image;
		}
		m_Images.clear();
	}

	//!
	//! Get the images in the root folder
	//!
	const QVector< Media * > &	MediaModel::GetImages(void) const
	{
		if (m_Dirty == true)
		{
			QDir dir(m_Root);
			for (const auto & file : dir.entryList(QDir::Files, QDir::Name))
			{
				if (IsMedia(file) == true)
				{
					m_Images.push_back(NEW Media(dir.absoluteFilePath(file)));
				}
			}

			// reset the dirty flag
			m_Dirty = false;
		}
		return m_Images;
	}

	//!
	//! Get the index from a path
	//!
	QModelIndex MediaModel::getIndexByPath(const QString & path) const
	{
		int index = 0;
		for (const Media * image : m_Images)
		{
			if (image->GetPath() == path)
			{
				return this->createIndex(index, 0, const_cast< Media * >(image));
			}
			++index;
		}
		return QModelIndex();
	}

	//!
	//! Get the previous index
	//!
	QModelIndex MediaModel::getPreviousIndex(const QModelIndex & index) const
	{
		if (index.isValid() == false || index.row() <= 1)
		{
			return QModelIndex();
		}
		return this->createIndex(index.row() - 1, 0, m_Images.at(index.row() - 1));
	}

	//!
	//! Get the next index
	//!
	QModelIndex MediaModel::getNextIndex(const QModelIndex & index) const
	{
		if (index.isValid() == false || index.row() >= m_Images.size() - 1)
		{
			return QModelIndex();
		}
		return this->createIndex(index.row() + 1, 0, m_Images.at(index.row() + 1));
	}

	//!
	//! Get an image
	//!
	Media * MediaModel::getImage(const QModelIndex & index) const
	{
		return index.isValid() == true ? static_cast< Media * >(index.internalPointer()) : nullptr;
	}

	//!
	//! Get the roles supported by this model
	//!
	QHash< int, QByteArray > MediaModel::roleNames(void) const
	{
		QHash< int, QByteArray > roles;
		roles.insert(Qt::DisplayRole,	"name");
		roles.insert(Qt::UserRole,		"path");
		return roles;
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

		Media * image = static_cast< Media * >(index.internalPointer());
		Q_ASSERT(image != nullptr);

		switch (role)
		{
			case Qt::DisplayRole:	return image->GetName();
			case Qt::UserRole:		return image->GetPath();
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
		return this->createIndex(row, column, this->GetImages().at(row));
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
		return parent.isValid() == true ? 0 : this->GetImages().size();
	}

	//!
	//! Get the number of columns.
	//!
	int MediaModel::columnCount(const QModelIndex & /* parent */) const
	{
		return 1;
	}

} // namespace MediaViewerLib
