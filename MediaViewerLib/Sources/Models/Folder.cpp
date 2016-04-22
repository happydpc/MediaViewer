#include "MediaViewerLibPCH.h"
#include "FolderModel.h"
#include "Folder.h"
#include "Media.h"
#include "Utils/Job.h"


namespace MediaViewerLib
{

	//!
	//! Constructor.
	//!
	Folder::Folder(const QString & path, const Folder * parent)
		: m_Parent(parent)
		, m_ImageCount(0)
		, m_Dirty(true)
	{
		this->SetPath(path);
	}

	//!
	//! Copy constructor
	//!
	Folder::Folder(const Folder & other)
		: m_Parent(other.m_Parent)
		, m_ImageCount(0)
		, m_Dirty(true)
	{
		this->SetPath(other.m_Path);
	}

	//!
	//! Destructor.
	//!
	Folder::~Folder()
	{
		for (Folder * folder : m_Children)
		{
			DELETE folder;
		}
	}

	//!
	//! Get the folder's path
	//!
	const QString & Folder::GetPath(void) const
	{
		return m_Path;
	}

	//!
	//! Set the folder's path
	//!
	void Folder::SetPath(const QString & path)
	{
		if (m_Path != path)
		{
			m_Path = path;
			m_Dirty = true;

			// get the name
			QDir dir = QDir(path);
			m_Name = dir.dirName();

			// drive letters
			if (m_Name.isEmpty() == true)
			{
				m_Name = dir.absolutePath();
			}

			// notify
			emit pathChanged(m_Path);
			emit nameChanged(m_Name);

			// update the images
			this->UpdateImages();
		}
	}

	//!
	//! Get the folder's name
	//!
	const QString & Folder::GetName(void) const
	{
		return m_Name;
	}

	//!
	//! Get the number of images in this folder
	//!
	int Folder::GetImageCount(void) const
	{
		return m_ImageCount;
	}

	//!
	//! Get the folder's parent
	//!
	const Folder * Folder::GetParent(void) const
	{
		return m_Parent;
	}

	//!
	//! Get the children
	//!
	const QVector< Folder * > & Folder::GetChildren(void) const
	{
		this->UpdateChildren();
		return m_Children;
	}

	//!
	//! Update the children list
	//!
	void Folder::UpdateChildren(void) const
	{
		if (m_Dirty == true)
		{
			// initialize the list of children
			QDir dir(m_Path);
			if (dir.exists() == true)
			{
				for (const auto & child : dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::NoSort))
				{
					m_Children.push_back(NEW Folder(dir.absoluteFilePath(child.filePath()), this));
				}
			}

			// reset dirty flag
			m_Dirty = false;
		}
	}

	//!
	//! Update the number of images in the folder
	//!
	void Folder::UpdateImages(void) const
	{
		NEW Job([&] (void) {
			// reset the image count
			m_ImageCount = 0;

			// initialize the list of children, and count images
			QDir dir(m_Path);
			if (dir.exists() == true)
			{
				for (const auto & child : dir.entryInfoList(QDir::Files, QDir::NoSort))
				{
					if (Media::IsMedia(child.fileName()))
					{
						++m_ImageCount;
					}
				}
			}

			// notify
			emit imageCountChanged(m_ImageCount);
		});
	}

} // namespace MediaViewerLib
