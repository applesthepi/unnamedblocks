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
	if (PreSizeUpdate(size, m_SuperBounds))
	{
		m_Size = size;
		PostSizeUpdate();
	}
}

template <typename T>
void ISizeable<T>::SetSizeMax()
{
	if (PreSizeUpdate(m_SuperBounds, m_SuperBounds))
	{
		m_Size = m_SuperBounds;
		PostSizeUpdate();
	}
}

template <typename T>
void ISizeable<T>::SetSuperBounds(const glm::vec<2, T>& bounds)
{
	if (PreSizeUpdate(m_Size, bounds))
	{
		m_SuperBounds = bounds;
		PostSizeUpdate();
	}
}

template <typename T>
glm::vec<2, T> ISizeable<T>::GetSize()
{
	PreSizePull();
	return m_Size;
}

template <typename T>
glm::vec<2, T> ISizeable<T>::GetSuperBounds()
{
	PreSizePull();
	return m_SuperBounds;
}

template <typename T>
bool ISizeable<T>::PreSizeUpdate(const glm::vec<2, T>& size, const glm::vec<2, T>& bounds)
{
	return true;
}

template <typename T>
void ISizeable<T>::PostSizeUpdate()
{
	
}

template <typename T>
void ISizeable<T>::PreSizePull()
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