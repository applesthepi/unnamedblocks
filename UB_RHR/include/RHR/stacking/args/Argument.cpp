#include "Argument.hpp"

#include "stacking/Block.hpp"

Argument::Argument(const Color& blockColor)
	: m_Mode(BlockArgumentVariableMode::RAW)//, m_Next(false)
	, m_BlockColor(blockColor)
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

int32_t Argument::Padding = Block::Padding * 4;

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
