#include "IPositionable.hpp"

template <typename T>
IPositionable<T>::IPositionable()
	: m_Position(glm::vec<2, T>())
	, m_SuperOffset(m_Position(glm::vec<2, T>()))
{

}

template <typename T>
void IPositionable<T>::SetPosition(const glm::vec<2, T>&& position)
{
	if (OnPositionUpdate(position, m_SuperOffset)) m_Position = std::move(position);
}

template <typename T>
void IPositionable<T>::SetSuperOffset(const glm::vec<2, T>&& offset)
{
	if (OnPositionUpdate(m_Position, offset)) m_SuperOffset = std::move(offset);
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
bool IPositionable<T>::OnPositionUpdate(const glm::vec<2, T>&& position, const glm::vec<2, T>&& offset)
{
	return true;
}

template <typename T>
void IPositionable<T>::OnPositionPull()
{

}