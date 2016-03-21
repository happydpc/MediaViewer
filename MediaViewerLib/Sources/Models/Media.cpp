#include "MediaViewerLibPCH.h"
#include "MediaModel.h"
#include "Media.h"


namespace MediaViewerLib
{

	//!
	//! Constructor.
	//!
	Media::Media(const QString & path)
		: m_Path(path)
		, m_Name(QFileInfo(path).fileName())
		, m_Type(GetMediaType(path))
	{
	}

	//!
	//! Copy constructor
	//!
	Media::Media(const Media & other)
		: m_Path(other.m_Path)
		, m_Name(other.m_Name)
		, m_Type(other.m_Type)
	{
	}

	//!
	//! Destructor.
	//!
	Media::~Media(void)
	{
	}

	//!
	//! Get the media's path
	//!
	const QString & Media::GetPath(void) const
	{
		return m_Path;
	}

	//!
	//! Get the media's name
	//!
	const QString & Media::GetName(void) const
	{
		return m_Name;
	}

	//!
	//! Get the media's type
	//!
	MediaType Media::GetType(void) const
	{
		return m_Type;
	}

} // namespace MediaViewerLib
