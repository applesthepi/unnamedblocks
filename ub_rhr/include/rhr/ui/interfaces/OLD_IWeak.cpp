#include "IWeak.hpp"

template <typename T>
IWeak<T>::IWeak()
	:m_WeakSet(false)
{

}

template <typename T>
void IWeak<T>::SetWeak(std::weak_ptr<T> weak)
{
	m_Weak = weak;
	m_WeakSet = true;
}

template <typename T>
bool IWeak<T>::IsWeak()
{
	if (!m_WeakSet)
		Logger::Warn("weak enforcement failed test");

	return m_WeakSet;
}