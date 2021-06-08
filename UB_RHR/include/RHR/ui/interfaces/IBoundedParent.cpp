#include "IBoundedParent.hpp"

IBoundedParent::IBoundedParent(IPositionable<int32_t>* positionable, ISizeable<int32_t>* sizeable)
	:m_Positionable(positionable), m_Sizeable(sizeable)
{

}

void IBoundedParent::SetPositionable(IPositionable<int32_t>* positionable)
{
	m_Positionable = positionable;
}

void IBoundedParent::SetSizeable(ISizeable<int32_t>* sizeable)
{
	m_Sizeable = sizeable;
}

IPositionable<int32_t>* IBoundedParent::GetPositionable()
{
	return m_Positionable;
}

ISizeable<int32_t>* IBoundedParent::GetSizeable()
{
	return m_Sizeable;
}