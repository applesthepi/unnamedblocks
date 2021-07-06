#pragma once
#include "config.h"
/*
#include "stacking/args/Argument.hpp"
#include "registries/UIRegistry.hpp"

#include <Cappuccino/Utils.hpp>
#include <Espresso/InputHandler.hpp>

#define BOOL_GEOMETRY_REACH ((Global::BlockHeight - Global::BlockBorder) / 2.0f)

class ArgumentBool : public Argument
{
public:
	/// TODO
	ArgumentBool(bool canSwitch);

	glm::vec<2, float> LCir(float angle);

	glm::vec<2, float> RCir(float angle);

	/// How to interpret the data.
	/// \return Type of argument.
	BlockArgumentType GetType() override;

	/// Get width for surrounding argument spacing.
	/// \return Width of argument.
	uint32_t GetWidth() override;

	/// Whether or not the argument contains data for storing.
	/// \return Has data.
	bool HasData() override;

	/// Select the argument and enable it.
	void Select() override;

	/// When the focus turns off of the argument.
	void UnSelect() override;
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

	Field m_field;
	std::function<void()> m_tab;
	sf::RenderTexture m_textRendered;
};
*/