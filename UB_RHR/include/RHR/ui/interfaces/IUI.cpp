#include "IUI.hpp"

IUI::IUI()
	:m_Virtual(false), m_Renderable(std::make_shared<IRenderable>()), m_Updatable(std::make_shared<IUpdatable>())
{
	m_Renderable->SetWeak(m_Renderable);
}

void IUI::SetupVirtualFunctions(IUI* iui)
{
	m_Renderable->SetupVirtualFunctions((void(*)())(iui->* & IUI::OnRender), (void(*)())(iui->* & IUI::OnUpdateBuffers), (void(*)())(iui->* & IUI::OnReloadSwapChain));
	m_Virtual = true;
}

std::shared_ptr<IRenderable> IUI::GetRenderable()
{
	if (!IsVirtual())
		return nullptr;

	return m_Renderable;
}

std::shared_ptr<IUpdatable> IUI::GetUpdatable()
{
	if (!IsVirtual())
		return nullptr;

	return m_Updatable;
}

bool IUI::IsVirtual()
{
	if (!m_Virtual)
	{
		Logger::Fatal("check for IUI::IsVirtual(); failed");
		return false;
	}

	return true;
}