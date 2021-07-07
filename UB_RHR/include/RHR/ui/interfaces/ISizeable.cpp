#include "ISizeable.hpp"

template <typename T>
ISizeable<T>::ISizeable()
	: m_Size(glm::vec<2, T>())
	, m_SuperBounds(glm::vec<2, T>())
{

}

template <typename T>
void ISizeable<T>::SetSize(const glm::vec<2, T>& size)
{
	if (OnSizeUpdate(size, m_SuperBounds)) m_Size = size;
}

template <typename T>
void ISizeable<T>::SetSizeMax()
{
	if (OnSizeUpdate(m_SuperBounds, m_SuperBounds)) m_Size = m_SuperBounds;
}

template <typename T>
void ISizeable<T>::SetSuperBounds(const glm::vec<2, T>& bounds)
{
	if (OnSizeUpdate(m_Size, bounds)) m_SuperBounds = bounds;
}

template <typename T>
glm::vec<2, T> ISizeable<T>::GetSize()
{
	OnSizePull();
	return m_Size;
}

template <typename T>
glm::vec<2, T> ISizeable<T>::GetSuperBounds()
{
	OnSizePull();
	return m_SuperBounds;
}

template <typename T>
bool ISizeable<T>::OnSizeUpdate(const glm::vec<2, T>& size, const glm::vec<2, T>& bounds)
{
	return true;
}

template <typename T>
void ISizeable<T>::OnSizePull()
{

}

template class ISizeable<int64_t>;
template class ISizeable<int32_t>;
template class ISizeable<int16_t>;
template class ISizeable<int8_t>;

template class ISizeable<uint64_t>;
template class ISizeable<uint32_t>;
template class ISizeable<uint16_t>;
template class ISizeable<uint8_t>;

template class ISizeable<double>;
template class ISizeable<float>;