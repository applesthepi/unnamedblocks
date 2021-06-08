#include "ISizeable.hpp"

template <typename T>
void ISizeable<T>::SetSize(const glm::vec<2, T>&& size)
{
	if (OnSetSize(size)) m_Size = std::move(size);
}

template <typename T>
glm::vec<2, T> ISizeable<T>::GetSize()
{
	OnGetSize();
	return m_Size;
}

template <typename T>
bool ISizeable<T>::OnSetSize(const glm::vec<2, T>&& size)
{
	return true;
}

template <typename T>
void ISizeable<T>::OnGetSize()
{

}