#include "IEnableable.hpp"

IEnableable::IEnableable(bool enabled)
	:m_Enabled(enabled)
{

}

void IEnableable::SetEnabled(bool enabled)
{
	m_Enabled = enabled;
}

bool IEnableable::GetEnabled()
{
	return m_Enabled;
}