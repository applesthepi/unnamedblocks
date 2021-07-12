#include "IRenderable.hpp"

#include "ui/Renderer.hpp"

IRenderable::IRenderable()
	: m_Dirty(false)/*, m_Virtual(false), m_Render(nullptr), m_UpdateBuffers(nullptr), m_ReloadSwapChain(nullptr)*/ , m_WeakSet(false)
{
}

void IRenderable::SetWeak(std::weak_ptr<IRenderable>&& weak)
{
	m_Weak = std::move(weak);
	m_WeakSet = true;

	OnSetWeak();
}

//void IRenderable::SetupVirtualFunctions(void(*render)(), void(*updateBuffers)(), void(*reloadSwapChain)())
//{
//	m_Render = render;
//	m_UpdateBuffers = updateBuffers;
//	m_ReloadSwapChain = reloadSwapChain;
//	m_Virtual = true;
//}

void IRenderable::Render()
{
	if (!IsWeak()/* || !IsVirtual()*/)
		return;

	OnRender();
}

void IRenderable::UpdateBuffers()
{
	if (!IsWeak()/* || !IsVirtual()*/)
		return;

	m_Dirty = false;
	OnUpdateBuffers();
}

void IRenderable::ReloadSwapChain()
{
	if (!IsWeak()/* || !IsVirtual()*/)
		return;

	OnReloadSwapChain();
}

void IRenderable::OnRender()
{

}

void IRenderable::OnUpdateBuffers()
{

}

void IRenderable::OnReloadSwapChain()
{

}

void IRenderable::OnSetWeak()
{

}

void IRenderable::MarkDirty()
{
	if (!IsWeak()/* || !IsVirtual()*/ || m_Dirty)
		return;

	m_Dirty = true;
	Renderer::AddDirty(m_Weak);
}

void IRenderable::ClearDirty()
{
	if (!IsWeak()/* || !IsVirtual()*/)
		return;

	m_Dirty = false;
}

std::weak_ptr<IRenderable>& IRenderable::GetWeak()
{
	return m_Weak;
}

//bool IRenderable::IsVirtual()
//{
//	if (!m_Virtual)
//	{
//		Logger::Warn("check for IRenderable::IsVirtual(); failed");
//		return false;
//	}
//
//	return true;
//}

bool IRenderable::IsWeak()
{
	if (!m_WeakSet)
	{
		Logger::Warn("check for IRenderable::IsWeak(); failed");
		return false;
	}

	return true;
}