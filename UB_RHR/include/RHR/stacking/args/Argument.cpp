#include "Argument.hpp"

Argument::Argument()
	:m_Mode(BlockArgumentVariableMode::RAW)//, m_Next(false)
{
	
}

void Argument::SetData(const std::string& data)
{
	m_Data = data;
	OnSetData();
}

const std::string& Argument::GetData()
{
	return m_Data;
}

void Argument::SetMode(BlockArgumentVariableMode mode)
{
	m_Mode = mode;
}

BlockArgumentVariableMode Argument::GetMode()
{
	return m_Mode;
}

// bool Argument::PullNext()
// {
// 	return m_Next;
// }

BlockArgumentType Argument::GetType()
{
	return BlockArgumentType::TEXT;
}

uint32_t Argument::GetWidth()
{
	return 50;
}

bool Argument::HasData()
{
	return false;
}

void Argument::Select()
{

}

void Argument::UnSelect()
{

}

void Argument::OnRender()
{

}

void Argument::OnUpdateBuffers()
{

}

void Argument::OnReloadSwapChain()
{

}

void Argument::OnFrameUpdate(double deltaTime)
{

}

void Argument::OnSetData()
{

}