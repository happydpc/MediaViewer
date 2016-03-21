#include "MediaViewerLibPCH.h"
#include "Job.h"


namespace MediaViewerLib
{

	//!
	//! Constructor.
	//!
	//! @param job
	//!		The job to execute.
	//!
	Job::Job(const std::function< void (void) > & job)
		: m_Job(job)
	{
		QThreadPool::globalInstance()->start(this);
	}

	//!
	//! Reimplemented from QRunnable::run.
	//!
	void Job::run(void)
	{
		m_Job();
	}

} // namespace MediaViewerLib
