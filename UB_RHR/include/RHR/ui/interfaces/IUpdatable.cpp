#include "IUpdatable.hpp"

IUpdatable::IUpdatable()
	//:m_Virtual(false), m_FrameUpdate(nullptr)
{

}

//void IUpdatable::SetupVirtualFunctions(void(*frameUpdate)(double deltaTime))
//{
//	m_FrameUpdate = frameUpdate;
//	m_Virtual = true;
//}

void IUpdatable::FrameUpdate(double deltaTime)
{
	//if (!IsVirtual())
	//	return;

	//m_FrameUpdate(deltaTime);
}

//bool IUpdatable::IsVirtual()
//{
//	if (!m_Virtual)
//	{
//		Logger::Warn("check for IUpdatable::IsVirtual(); failed");
//		return false;
//	}
//
//	return true;
//}