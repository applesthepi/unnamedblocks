#include "IPositionable.hpp"

template <typename T>
void IPositionable<T>::SetPosition(const glm::vec<2, T>&& position)
{
	if (OnSetPosition(position)) m_Position = std::move(position);
}

template <typename T>
glm::vec<2, T> IPositionable<T>::GetPosition()
{
	OnGetPosition();
	return m_Position;
}

template <typename T>
bool IPositionable<T>::OnSetPosition(const glm::vec<2, T>&& position)
{
	return true;
}

template <typename T>
void IPositionable<T>::OnGetPosition()
{

}