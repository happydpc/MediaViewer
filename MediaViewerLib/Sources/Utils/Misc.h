#ifndef __UTILS_MISC_H__
#define	__UTILS_MISC_H__


namespace MediaViewerLib
{
	namespace Utils
	{

		template< typename ContainerType, typename Type > int IndexOf(const ContainerType & container, const Type & element);

	} // namespace Utils
} // namespace MediaViewerLib


#include "Misc.inl"


#endif // __UTILS_MISC_H__
