#pragma once
#include "config.h"
#include "ui/interfaces/IWeak.hpp"

#include <Cappuccino/Utils.hpp>

/// Interface for renderable objects.

/// Parent interface with virtual functions REQUIRED.
/// Upstream extenders can access the std::shared_ptr<IRenderable>
/// that can then call upstream callbacks once external
/// calls to the public functions are preformed.

class IRenderable : public IWeak<IRenderable>
{
public:
	/// Runs initializer list.
	IRenderable();

	/// Setup upstream callbacks.
	void SetupVirtualFunctions(void(*render)(), void(*updateBuffers)(), void(*reloadSwapChain)());

	/// Add draw calls to cmd buffer prebound by Renderer.
	void Render();

	/// Updates mesh on cpu side.
	void UpdateBuffers();

	/// Regenerates descriptor sets including uniforms.
	void ReloadSwapChain();

	/// Marks IRenderable as dirty. UpdateBuffers will be called next frame. Expected only main thread acccess; non-atomic.
	void MarkDirty();

	/// Clears IRenderable's dirty tag.
	void ClearDirty();
private:
	/// Check for virtual
	bool IsVirtual();

	/// Guarantees only one IRenderable::MarkDirty(); call.
	bool m_Dirty;

	/// Garantees upstream callbacks exist
	bool m_Virtual;

	/// Render upstream callback
	void(*m_Render)();

	/// UpdateBuffers upstream callback
	void(*m_UpdateBuffers)();

	/// ReloadSwapChain upstream callback
	void(*m_ReloadSwapChain)();
};