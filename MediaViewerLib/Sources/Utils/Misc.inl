#ifndef __UTILS_MISC_INL__
#define	__UTILS_MISC_INL__


namespace MediaViewerLib
{
	namespace Utils
	{

		//!
		//! Get the index of an element in a container.
		//!
		//! @param container
		//!		The container.
		//!
		//! @param element
		//!		The element.
		//!
		//! @return
		//!		The 0 based index of the element in the list if found, -1 otherwise.
		//!
		template< typename ContainerType, typename Type >
		int IndexOf(const ContainerType & container, const Type & element)
		{
			auto iterator = std::find(container.begin(), container.end(), element);
			return iterator == container.end() ? -1 : int(std::distance(container.begin(), iterator));
		}

	} // namespace Utils
} // namespace MediaViewerLib


#endif // __UTILS_MISC_INL__
