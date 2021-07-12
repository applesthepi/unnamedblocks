#include "IPositionable.hpp"

template <size_t S, typename T>
IPositionable<S, T>::IPositionable()
	: m_Position(glm::vec<S, T>())
	, m_SuperOffset(glm::vec<S, T>())
{

}

template <size_t S, typename T>
void IPositionable<S, T>::SetPosition(const glm::vec<S, T>& position)
{
	if (PrePositionUpdate(position, m_SuperOffset))
	{
		m_Position = position;
		PostPositionUpdate();
	}
}

template <size_t S, typename T>
void IPositionable<S, T>::SetSuperOffset(const glm::vec<S, T>& offset)
{
	if (PrePositionUpdate(m_Position, offset))
	{
		m_SuperOffset = offset;
		PostPositionUpdate();
	}
}

template <size_t S, typename T>
glm::vec<S, T> IPositionable<S, T>::GetPosition()
{
	PrePositionPull();
	return m_Position;
}

template <size_t S, typename T>
glm::vec<S, T> IPositionable<S, T>::GetSuperOffset()
{
	PrePositionPull();
	return m_Position;
}

template <size_t S, typename T>
bool IPositionable<S, T>::PrePositionUpdate(const glm::vec<S, T>& position, const glm::vec<S, T>& offset)
{
	return true;
}

template <size_t S, typename T>
void IPositionable<S, T>::PostPositionUpdate()
{
	
}

template <size_t S, typename T>
void IPositionable<S, T>::PrePositionPull()
{

}

template class IPositionable<2, int64_t>;
template class IPositionable<2, int32_t>;
template class IPositionable<2, int16_t>;
template class IPositionable<2, int8_t>;

template class IPositionable<2, uint64_t>;
template class IPositionable<2, uint32_t>;
template class IPositionable<2, uint16_t>;
template class IPositionable<2, uint8_t>;

template class IPositionable<2, double>;
template class IPositionable<2, float>;

template class IPositionable<3, double>;
template class IPositionable<3, float>;