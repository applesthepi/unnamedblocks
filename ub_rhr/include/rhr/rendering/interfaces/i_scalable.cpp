#include "IScalable.hpp"

template <typename T>
void IScalable<T>::SetScale(const glm::vec<2, T>&& scale)
{
	if (OnSetScale(scale)) m_Scale = std::move(scale);
}

template <typename T>
glm::vec<2, T> IScalable<T>::GetScale()
{
	OnGetScale();
	return m_Scale;
}

template <typename T>
bool IScalable<T>::OnSetScale(const glm::vec<2, T>&& scale)
{
	return true;
}

template <typename T>
void IScalable<T>::OnGetScale()
{

}