#pragma once
#include "config.h"

#include <Cappuccino/Utils.hpp>

/// Interface for renderable objects.

/// Parent interface with virtual functions RECOMMENDED.
/// Upstream extenders can access the std::shared_ptr<IRenderable>
/// that can then call upstream callbacks once external
/// calls to the public functions are preformed.

class IRenderable
{
public:
	/// Runs initializer list.
	IRenderable();

	/// REQUIRED Sets weak.
	void SetWeak(const std::weak_ptr<IRenderable>&& weak);

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

	/// Gets the weak reference.
	/// \return Weak reference to itself.
	std::weak_ptr<IRenderable>& GetWeak();
private:
	/// Check for virtual.
	bool IsVirtual();

	/// Check for weak.
	bool IsWeak();

	/// Guarantees only one IRenderable::MarkDirty(); call.
	bool m_Dirty;

	/// Garantees upstream callbacks exist.
	bool m_Virtual;

	/// Render upstream callback.
	void(*m_Render)();

	/// UpdateBuffers upstream callback.
	void(*m_UpdateBuffers)();

	/// ReloadSwapChain upstream callback.
	void(*m_ReloadSwapChain)();

	/// Weak reference.
	std::weak_ptr<IRenderable> m_Weak;

	/// Flag if weak is set.
	bool m_WeakSet;
};