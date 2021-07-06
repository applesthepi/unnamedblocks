#include "IPositionable.hpp"

template <typename T>
IPositionable<T>::IPositionable()
	: m_Position(glm::vec<2, T>())
	, m_SuperOffset(glm::vec<2, T>())
{

}

template <typename T>
void IPositionable<T>::SetPosition(const glm::vec<2, T>& position)
{
	if (OnPositionUpdate(position, m_SuperOffset)) m_Position = position;
}

template <typename T>
void IPositionable<T>::SetSuperOffset(const glm::vec<2, T>& offset)
{
	if (OnPositionUpdate(m_Position, offset)) m_SuperOffset = offset;
}

template <typename T>
glm::vec<2, T> IPositionable<T>::GetPosition()
{
	OnPositionPull();
	return m_Position;
}

template <typename T>
glm::vec<2, T> IPositionable<T>::GetSuperOffset()
{
	OnPositionPull();
	return m_Position;
}

template <typename T>
bool IPositionable<T>::OnPositionUpdate(const glm::vec<2, T>& position, const glm::vec<2, T>& offset)
{
	return true;
}

template <typename T>
void IPositionable<T>::OnPositionPull()
{

}

template class IPositionable<int64_t>;
template class IPositionable<int32_t>;
template class IPositionable<int16_t>;
template class IPositionable<int8_t>;

template class IPositionable<uint64_t>;
template class IPositionable<uint32_t>;
template class IPositionable<uint16_t>;
template class IPositionable<uint8_t>;

template class IPositionable<double>;
template class IPositionable<float>;