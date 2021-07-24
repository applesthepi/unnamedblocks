#pragma once
#include "config.h"

#include "rhr/stacking/arguments/argument.hpp"
#include "rhr/rendering/interfaces/i_renderable.hpp"
#include "rhr/rendering/interfaces/i_updateable.hpp"
#include "rhr/rendering/interfaces/i_positionable.hpp"
#include "rhr/rendering/interfaces/i_sizeable.hpp"
#include "rhr/rendering/objects/rectangle.hpp"

#include <cappuccino/utils.hpp>
#include <cappuccino/mod_block/block.hpp>
#include <espresso/mod/category.hpp>

namespace rhr::stack
{
///
class block : public rhr::render::interfaces::i_renderable, public rhr::render::interfaces::i_updateable, public rhr::render::interfaces::i_positionable<2, i32>, public rhr::render::interfaces::i_sizeable<2, i32>
{
public:
	///
	block(const std::string& unlocalized_name);

	///
	const std::vector<std::shared_ptr<rhr::stack::argument::argument>>& GetArguments();

	///
	u32 GetWidth();

	///
	const ModBlock* GetModBlock();

	///
	const ModCatagory* GetModCategory();

	///
	void FrameUpdate(double deltaTime) override;

	///
	static i16 Padding;

	///
	static i16 Height;

	///
	static i16 HeightContent;
private:
	///
	void OnRender() override;

	///
	void OnUpdateBuffers() override;

	///
	void OnReloadSwapChain() override;

	///
	void PostPositionUpdate() override;

	///
	void UpdateArguments();

	///
	void UpdateWidth();

	///
	uint32_t m_width;

	///
	std::vector<std::shared_ptr<rhr::stack::argument::argument>> m_arguments;

	///
	const ModBlock* m_modBlock;

	///
	const ModCatagory* m_modCategory;

	///
	std::shared_ptr<rhr::render::object::rectangle> m_Background;
};
}