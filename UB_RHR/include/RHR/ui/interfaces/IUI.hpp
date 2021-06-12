#pragma once
#include "config.h"

#include "ui/interfaces/IRenderable.hpp"
#include "ui/interfaces/IUpdatable.hpp"

#include <Cappuccino/Utils.hpp>

/// Interface for any elements that need IRenderable & IUpdatable

class IUI
{
public:
	/// Constructs IRenderable & IUpdatable.
	IUI();

	/// Links extender to children.
	void SetupVirtualFunctions(IUI* iui);

	/// Retrives a shared_ptr to IRenderable.
	std::shared_ptr<IRenderable> GetRenderable();

	/// Retrives a shared_ptr to IUpdatable.
	std::shared_ptr<IUpdatable> GetUpdatable();
protected:
	/// Add draw calls to cmd buffer prebound by Renderer.
	virtual void OnRender();

	/// Updates mesh on cpu side.
	virtual void OnUpdateBuffers();

	/// Regenerates descriptor sets including uniforms.
	virtual void OnReloadSwapChain();

	/// Runs every frame before rendering.
	/// \param Seconds since last frame.
	virtual void OnFrameUpdate(double deltaTime);
private:
	/// Check for virtual.
	bool IsVirtual();

	/// Garantees upstream callbacks exist.
	bool m_Virtual;

	/// IRenderable used by the class extender.
	std::shared_ptr<IRenderable> m_Renderable;

	/// IUpdatable used by the class extender.
	std::shared_ptr<IUpdatable> m_Updatable;
};