#pragma once
#include "config.h"

#include "stacking/args/Argument.hpp"
#include "registries/UIRegistry.hpp"
#include "ui/RenderText.hpp"

#include <Cappuccino/Utils.hpp>
#include <Espresso/InputHandler.hpp>

class ArgumentText : public Argument
{
public:
	ArgumentText(const Color& color);

	/// How to interpret the data.
	/// \return Type of argument.
	BlockArgumentType GetType() override;

	/// Get width for surrounding argument spacing.
	/// \return Width of argument.
	uint32_t GetWidth() override;

	/// Whether or not the argument contains data for storing.
	/// \return Has data.
	bool HasData() override;
private:
	/// Add draw calls to cmd buffer prebound by Renderer.
	void OnRender() override;

	/// Updates mesh on cpu side.
	void OnUpdateBuffers() override;

	/// Regenerates descriptor sets including uniforms.
	void OnReloadSwapChain() override;

	/// Runs every frame before rendering.
	/// \param Seconds since last frame.
	void OnFrameUpdate(double deltaTime) override;

	void PostPositionUpdate() override;

	/// Sets data. Data of the argument is stored as a string.
	void OnSetData() override;
	
	/// Renderable text element.
	std::shared_ptr<vui::RenderText> m_Text;
};
