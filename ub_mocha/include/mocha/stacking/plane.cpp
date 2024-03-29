#include "plane.hpp"

// #include "lungo/handlers/context.hpp"
// #include "lungo/renderer.hpp"

//static void primary_plane_mouse_button(
//	glm::vec<2, i32> position, f32 scroll, mac::input::mouse_operation operation, rhr::handler::input::mouse_button button, void* data)
//{
//	rhr::stack::plane::primary_plane->mouse_button(position, scroll, operation, button);
//}
//
//static void toolbar_plane_mouse_button(
//	glm::vec<2, i32> position, f32 scroll, mac::input::mouse_operation operation, rhr::handler::input::mouse_button button, void* data)
//{
//	rhr::stack::plane::toolbar_plane->mouse_button(position, scroll, operation, button);
//}

rhr::stack::plane::plane(mac::window::state* window_state, bool toolbar)
	: m_toolbar(toolbar)
	, m_selected(false)
	, m_selected_context(false)
	, m_dragging_up(false)
	, m_dragging_snap(false)
	, m_background_shape(window_state, { 0, 0 }, { 0, 0 }, { 0.2f, 0.0f, 0.0f, 1.0f})
	, m_background_entity(&m_background_shape)
	, m_mouse_button_idx(0)
{
	cap_offset();
	// m_field.set_static_offset(get_static_offset());

	// m_background->initialize();
	// m_dragging_connecting_line->initialize();

	//if (toolbar)
	//	rhr::handler::input::register_mouse_callback(toolbar_plane_mouse_button, nullptr);
	//else
	//	rhr::handler::input::register_mouse_callback(primary_plane_mouse_button, nullptr);

	m_collections.reserve(5);
	m_collection_vanity.reserve(5);

#if 0
	m_mouse_button = [this](rhr::handler::input::mouse_button_data mouse_button_data)
	{
		mouse_button(mouse_button_data.position, mouse_button_data.scroll, mouse_button_data.operation, mouse_button_data.button);
	};
#endif

	m_mouse_button_idx = mac::input::register_mouse_callback(window_state->input_state, mac::input::layer_plane, [&](const mac::input::mouse_state& mouse_state){
		return mouse_button(mouse_state);
	});

	// m_mouse_button_idx = rhr::handler::input::register_mouse_callback(m_mouse_button, nullptr);

	m_context_callback = [](u8 idx)
	{
		if (idx == 0)
		{
			// stack duplicate

			// m_selectedCollection->add_stack(new rhr::stack::stack(*m_selectedStack));
		}
		else if (idx == 0)
		{
			// stack delete
		}
		else if (idx == 0)
		{
			// block duplicate
		}
		else if (idx == 0)
		{
			// block delete
		}
	};
}

void rhr::stack::plane::add_collection(
	std::shared_ptr<rhr::stack::collection>& collection, bool display_collection_vanity)
{
	update_child_transform(collection, i_ui::transform_update_spec_position | i_ui::transform_update_spec_size);
	m_collections.push_back(collection);
	m_collection_vanity.push_back(display_collection_vanity);

	collection->set_static_offset(get_static_offset());
	collection->display_vanity(display_collection_vanity);
}

const std::vector<std::shared_ptr<rhr::stack::collection>>& rhr::stack::plane::get_collections()
{
	return m_collections;
}

void rhr::stack::plane::delete_collection(u64 idx)
{
	m_collection_vanity.erase(m_collection_vanity.begin() + idx);
	m_collections.erase(m_collections.begin() + idx);
}

bool rhr::stack::plane::is_toolbar()
{
	return m_toolbar;
}

void rhr::stack::plane::delete_contents(bool disable_collections)
{
	if (disable_collections)
	{
		for (auto& collection : m_collections)
			collection->set_enabled(false);
	}

	m_collections.clear();
}

mac::input::capture rhr::stack::plane::mouse_button(const mac::input::mouse_state& mouse_state)
{
	if (mouse_state.operation == mac::input::mouse_operation::SCROLL_VERTICAL)
	{
		auto& virtual_position = get_position_virtual_absolute();
		auto& virtual_size	   = get_size_local();

		if (mouse_state.position.x >= virtual_position.x && mouse_state.position.x < virtual_position.x + virtual_size.x
			&& mouse_state.position.y >= virtual_position.y && mouse_state.position.y < virtual_position.y + virtual_size.y)
			get_static_offset()->y += static_cast<i32>(mouse_state.scroll * 100.0f);
	}
	else if (mouse_state.operation == mac::input::mouse_operation::SCROLL_HORIZONTAL)
	{
		auto& virtual_position = get_position_virtual_absolute();
		auto& virtual_size	   = get_size_local();

		if (mouse_state.position.x >= virtual_position.x && mouse_state.position.x < virtual_position.x + virtual_size.x
			&& mouse_state.position.y >= virtual_position.y && mouse_state.position.y < virtual_position.y + virtual_size.y)
			get_static_offset()->x += static_cast<i32>(mouse_state.scroll * 100.0f);
	}

	if (mouse_state.operation != mac::input::mouse_operation::PRESS && mouse_state.operation != mac::input::mouse_operation::RELEASE)
		return;

	if ((m_toolbar && rhr::stack::plane::primary_plane->dragging_stack())
		|| (!m_toolbar && rhr::stack::plane::toolbar_plane->dragging_stack()))
		return;

	glm::vec<2, f32> position = mouse_state.position - *get_static_offset();

	if (dragging_stack())
	{
		if (mouse_state.operation == mac::input::mouse_operation::RELEASE && !m_dragging_up)
		{
			// dragging and mouse released (used when dragging a stack)
			undrag(position);
		}
		else if (mouse_state.operation == mac::input::mouse_operation::PRESS && m_dragging_up)
		{
			// dragging and mouse pressed (used when duplicating stack)
			undrag(position);
		}
	}

	// TODO: Sub out into functionals here so there arnt so many indentions.

	i64 collection_max = 0;

	if (m_collections.empty())
		return;

	for (i64 ii = static_cast<i64>(m_collections.size()) - 1; ii >= 0; ii--)
	{
		u64 i = static_cast<u64>(ii);

		if (!m_collections[i]->get_enabled())
			continue;

		glm::vec<2, i32> collection_size	 = m_collections[i]->get_size_local();
		glm::vec<2, i32> collection_position = m_collections[i]->get_position_virtual_absolute();

		if (position.x > collection_position.x && position.x < collection_position.x + collection_size.x
			&& position.y > collection_position.y && position.y < collection_position.y + collection_size.y)
		{
			for (i64 aa = static_cast<i64>(m_collections[i]->get_stacks().size()) - 1; aa >= 0; aa--)
			{
				u64 a = static_cast<u64>(aa);

				glm::vec<2, i32> stack_size		= m_collections[i]->get_stacks()[a]->get_size_local();
				glm::vec<2, i32> stack_position = m_collections[i]->get_stacks()[a]->get_position_virtual_absolute();

				if (position.x > stack_position.x && position.x < stack_position.x + stack_size.x
					&& position.y > stack_position.y && position.y < stack_position.y + stack_size.y)
				{
					for (u64 b = 0; b < m_collections[i]->get_stacks()[a]->get_blocks().size(); b++)
					{
						glm::vec<2, i32> block_size =
							m_collections[i]->get_stacks()[a]->get_blocks()[b]->get_size_local();
						glm::vec<2, i32> block_position =
							m_collections[i]->get_stacks()[a]->get_blocks()[b]->get_position_virtual_absolute();

						if (position.x >= block_position.x && position.x < block_position.x + block_size.x
							&& position.y >= block_position.y && position.y < block_position.y + block_size.y)
						{
							if (mouse_state.operation == mac::input::mouse_operation::PRESS && !dragging_stack())
							{
								// not dragging and mouse down

								if (mouse_state.button == mac::input::mouse_button::LEFT)
								{
									unselect();

									if (m_collections[i]->get_stacks()[a]->get_blocks()[b]->drag_bounds(position))
										return;

									std::shared_ptr<rhr::stack::collection> active_collection =
										std::make_shared<rhr::stack::collection>(get_static_offset());

									// TODO: doing these 3 things is unnecessary. Make it so you can just CREATE the
									//  collection AS a child of the parent in one go.

									// TODO: mabye also make a require system so before the child can be used at
									//  all, it must have the data it requires.

									active_collection->set_static_offset(get_static_offset());
									update_child_transform(active_collection, 0);
									active_collection->initialize();
									update_child_transform(active_collection, i_ui::transform_update_spec_position | i_ui::transform_update_spec_size);

									std::shared_ptr<rhr::stack::stack> active_stack = m_collections[i]->get_stacks()[a];

									if (m_toolbar)
									{
										std::shared_ptr<rhr::stack::stack> dragging_stack =
											std::make_shared<rhr::stack::stack>(get_static_offset());
										dragging_stack->set_static_offset(get_static_offset());
										dragging_stack->initialize();

										const std::vector<std::shared_ptr<rhr::stack::block>>& blocks =
											active_stack->get_blocks();
										std::vector<std::shared_ptr<rhr::stack::block>> cloned_blocks;

										for (const auto& block : blocks)
										{
											std::shared_ptr<rhr::stack::block> cloned_block =
												std::make_shared<rhr::stack::block>(
													block->get_esp_block()->unlocalized_name);
											cloned_block->initialize();
											cloned_block->set_static_offset(get_static_offset());

											cloned_blocks.emplace_back(cloned_block);
										}

										dragging_stack->add_blocks(cloned_blocks);
										active_stack = dragging_stack;
									}

									if (b > 0)
									{
										if (m_toolbar)
										{
											latte::logger::error(
												latte::logger::level::EDITOR,
												"splitting stacks inside toolbar is not supported");
											return;
										}

										// create stack that is left behind (not the one picked up)

										std::shared_ptr<rhr::stack::stack> left_stack =
											std::make_shared<rhr::stack::stack>(get_static_offset());
										left_stack->set_position_local_physical(
											active_stack->get_position_local_physical(), false);
										left_stack->initialize();
										left_stack->update_transform(i_ui::transform_update_spec_position);

										for (u64 c = 0; c < b; c++)
											left_stack->add_block(m_collections[i]->get_stacks()[a]->get_blocks()[c]);

										for (u64 c = 0; c < b; c++)
											active_stack->remove_block(0);

										m_collections[i]->add_stack(left_stack, false);
										stack_position += glm::vec<2, i32>(
											0, static_cast<i32>(b) * static_cast<i32>(BLOCK_HEIGHT));
									}

									// active_collection->set_position_parent_physical(get_position_virtual_absolute(),
									// false);
									active_collection->set_position_local_physical(
										stack_position - active_collection->get_position_virtual_offset(), true);
									active_collection->set_size_local(stack_size, true);

									// m_dragging_connecting_line->set_color(
										// active_stack->get_blocks().front()->get_esp_category()->get_color());

									active_stack->set_position_parent_physical({0, 0}, false);
									active_stack->set_position_local_physical({0, 0}, true);

									if (!m_toolbar)
										m_collections[i]->remove_stack(a);

									active_collection->add_stack(active_stack, false);
									active_collection->size_to_stacks(true, false);

									drag_stack(active_collection, active_stack, false);

									if (m_collections[i]->get_stacks().empty())
										delete_collection(i);

									return;
								}
								else if (mouse_state.button == mac::input::mouse_button::RIGHT)
								{
									unselect();

									if (m_collections[i]->get_stacks()[a]->get_blocks()[b]->drag_bounds(position))
										return;

									std::weak_ptr<rhr::stack::collection> local_collection = m_collections[i];
									std::weak_ptr<rhr::stack::stack> local_stack = m_collections[i]->get_stacks()[a];
									std::weak_ptr<rhr::stack::block> local_block =
										m_collections[i]->get_stacks()[a]->get_blocks()[b];

									u64 local_collection_idx = i;
									u64 local_stack_idx		 = a;
									u64 local_block_idx		 = b;

#if 0
									rhr::handler::context::open(
										rhr::handler::context::flag::OBJECT_STACKING,
										[local_collection,
										local_stack,
										local_block,
										local_collection_idx,
										local_stack_idx,
										local_block_idx,
										stack_position,
										stack_size,
										this](RHR_HANDLER_CONTEXT_FLAG_PREFIX flag, u8 flag_menu_item)
										{
											if (flag == rhr::handler::context::flag::OBJECT_STACKING)
											{
												bool flag_delete_collection = false;

												switch (rhr::handler::context::flag::hashed_menu_object[flag_menu_item])
												{
												case rhr::handler::context::flag::menu_object::DUPLICATE_STACK:
													///////////////////////////////////////////////////////////

													if (auto stack = local_stack.lock())
													{
														std::shared_ptr<rhr::stack::collection> new_collection =
															std::make_shared<rhr::stack::collection>(get_static_offset());
														new_collection->initialize();
														update_child_transform(
															new_collection,
															i_ui::transform_update_spec_position
																| i_ui::transform_update_spec_size);

														std::shared_ptr<rhr::stack::stack> new_stack =
															std::make_shared<rhr::stack::stack>(get_static_offset());
														new_stack->initialize();

														// clone blocks

														const std::vector<std::shared_ptr<rhr::stack::block>>& blocks =
															stack->get_blocks();
														std::vector<std::shared_ptr<rhr::stack::block>> cloned_blocks;

														for (u64 i = local_block_idx; i < blocks.size(); i++)
														{
															std::shared_ptr<rhr::stack::block> cloned_block =
																std::make_shared<rhr::stack::block>(
																	blocks[i]->get_esp_block()->get_unlocalized_name());
															cloned_block->initialize();
															cloned_block->set_static_offset(get_static_offset());

															latte::serializer::node base_node;
															blocks[i]->serialize(base_node);
															cloned_block->deserialize(base_node);

															cloned_blocks.push_back(std::move(cloned_block));
														}

														new_stack->add_blocks(cloned_blocks);

														// transform

														// new_collection->set_position_parent_physical(get_position_virtual_absolute(),
														// false);
														new_collection->set_position_local_physical(
															stack_position
																- new_collection->get_position_virtual_offset()
																+ glm::vec<2, i32>(
																	0,
																	static_cast<i32>(local_block_idx)
																		* static_cast<i32>(BLOCK_HEIGHT)),
															false);
														new_collection->set_size_local(stack_size, true);

														m_dragging_connecting_line->set_color(new_stack->get_blocks()
																								.front()
																								->get_esp_category()
																								->get_color());

														new_stack->set_position_parent_physical({0, 0}, false);
														new_stack->set_position_local_physical({0, 0}, true);

														// submit

														new_collection->add_stack(new_stack, false);
														new_collection->size_to_stacks(true, false);

														drag_stack(new_collection, new_stack, true);
													}

													break;
												case handler::context::flag::menu_object::DELETE_STACK:
													///////////////////////////////////////////////////

													if (auto collection = local_collection.lock())
													{
														collection->remove_stack(local_stack_idx, local_block_idx);

														if (collection->get_stacks().empty())
															flag_delete_collection = true;
													}

													break;
												case handler::context::flag::menu_object::DUPLICATE_BLOCK:
													//////////////////////////////////////////////////////

													if (auto stack = local_stack.lock())
													{
														std::shared_ptr<rhr::stack::collection> new_collection =
															std::make_shared<rhr::stack::collection>(get_static_offset());
														new_collection->initialize();
														update_child_transform(
															new_collection,
															i_ui::transform_update_spec_position
																| i_ui::transform_update_spec_size);

														std::shared_ptr<rhr::stack::stack> new_stack =
															std::make_shared<rhr::stack::stack>(get_static_offset());
														new_stack->initialize();

														// clone block

														const std::vector<std::shared_ptr<rhr::stack::block>>& blocks =
															stack->get_blocks();
														std::shared_ptr<rhr::stack::block> cloned_block =
															std::make_shared<rhr::stack::block>(
																blocks[local_block_idx]
																	->get_esp_block()
																	->get_unlocalized_name());
														cloned_block->initialize();
														cloned_block->set_static_offset(get_static_offset());

														latte::serializer::node base_node;
														blocks[local_block_idx]->serialize(base_node);
														cloned_block->deserialize(base_node);

														// TODO: copy blocks fix for serialization
														//cloned_block->set_data(blocks[local_block_idx]->get_data());

														new_stack->add_block(cloned_block);

														// transform

														// new_collection->set_position_parent_physical(get_position_virtual_absolute(),
														// false);
														new_collection->set_position_local_physical(
															stack_position
																- new_collection->get_position_virtual_offset()
																+ glm::vec<2, i32>(
																	0,
																	static_cast<i32>(local_block_idx)
																		* static_cast<i32>(BLOCK_HEIGHT)),
															false);
														new_collection->set_size_local(stack_size, true);

														m_dragging_connecting_line->set_color(new_stack->get_blocks()
																								.front()
																								->get_esp_category()
																								->get_color());

														new_stack->set_position_parent_physical({0, 0}, false);
														new_stack->set_position_local_physical({0, 0}, true);

														// submit

														new_collection->add_stack(new_stack, false);
														new_collection->size_to_stacks(true, false);

														drag_stack(new_collection, new_stack, true);
													}

													break;
												case handler::context::flag::menu_object::DELETE_BLOCK:
													///////////////////////////////////////////////////

													if (auto collection = local_collection.lock())
													{
														collection->get_stacks()[local_stack_idx]->remove_block(
															local_block_idx);

														if (collection->get_stacks()[local_stack_idx]
																->get_blocks()
																.empty())
															collection->remove_stack(local_stack_idx);

														if (collection->get_stacks().empty())
															flag_delete_collection = true;
													}

													break;
												}

												if (flag_delete_collection)
													delete_collection(local_collection_idx);
											}
										}
									);
#endif
								}
							}

							// if block was bounded

							return;
						}
					}

					// if stack was bounded

					break;
				}
			}

			// if collection was bounded

			break;
		}
	}

	// collection_max returns early

	return mac::input::capture::BLOCK;
}
#if 0
rhr::handler::field& rhr::stack::plane::get_field()
{
	return m_field;
}
#endif
void rhr::stack::plane::render_master_pass()
{
	if (dragging_stack() || dragging_collection())
	{
		m_dragging_collection->render();

		// if (m_dragging_snap)
			// m_dragging_connecting_line->render();
	}
}

void rhr::stack::plane::ui_initialize()
{
	update_child_transform(m_background, i_ui::transform_update_spec_position);

	m_background->set_size_local(get_size_local(), true);
	m_background->set_color(espresso::color().from_u8({0, 0, 0, 255}));
	m_background->set_depth(rhr::render::renderer::depth_plane);

	update_child_transform(
		m_dragging_connecting_line, i_ui::transform_update_spec_position | i_ui::transform_update_spec_size);

	m_dragging_connecting_line->set_color(espresso::color().from_u8({255, 255, 255, 255}));
	m_dragging_connecting_line->set_depth(rhr::render::renderer::depth_argument_text);
	m_dragging_connecting_line->set_line_half_width(3);
}

void rhr::stack::plane::ui_transform_update(i_ui::transform_update_spec transform_update_spec)
{
	set_size_max(false);
	update_child_transform(m_background, i_ui::transform_update_spec_position | i_ui::transform_update_spec_size);

	for (auto& collection : m_collections)
		update_child_transform(collection, i_ui::transform_update_spec_position | i_ui::transform_update_spec_size);
}

void rhr::stack::plane::ui_frame_update(f64 delta_time)
{
	m_background->frame_update(delta_time);

	for (auto& collection : m_collections)
		collection->frame_update(delta_time);

	if (dragging_stack() || dragging_collection())
	{
		m_dragging_collection->frame_update(delta_time);
		m_dragging_connecting_line->frame_update(delta_time);
	}

	if (dragging_stack())
		dragging_stack_update();
}

void rhr::stack::plane::ui_render()
{
	for (auto& collection : m_collections)
		collection->render();

	m_field.render();
}

void rhr::stack::plane::ui_reload_swap_chain()
{
	m_background->reload_swap_chain();

	for (auto& collection : m_collections)
		collection->reload_swap_chain();

	if (dragging_stack() || dragging_collection())
	{
		m_dragging_collection->reload_swap_chain();
		m_dragging_connecting_line->reload_swap_chain();
	}

	m_field.reload_swap_chain();
}

void rhr::stack::plane::ui_update_buffers()
{}

void rhr::stack::plane::ui_chain_update_buffers()
{
	m_background->update_buffers();

	for (auto& collection : m_collections)
		collection->update_buffers();

	if (dragging_stack() || dragging_collection())
	{
		m_dragging_collection->update_buffers();
		m_dragging_connecting_line->update_buffers();
	}

	m_field.update_buffers();
}

void rhr::stack::plane::ui_static_offset_update()
{

}

void rhr::stack::plane::ui_serialize(latte::serializer::node& node)
{
	for (auto& collection : m_collections)
	{
		auto& child_node = node.children.emplace_back();
		collection->serialize(child_node);
	}
}

void rhr::stack::plane::ui_deserialize(latte::serializer::node& node)
{
	delete_contents();

	for (auto& child : node.children)
	{
		auto collection = std::make_shared<rhr::stack::collection>(get_static_offset());
		collection->initialize();
		add_collection(collection, true);
		collection->deserialize(child);
	}
}
std::shared_ptr<rhr::stack::plane> rhr::stack::plane::primary_plane;

std::shared_ptr<rhr::stack::plane> rhr::stack::plane::toolbar_plane;

void rhr::stack::plane::select(u64 collection, u64 stack, u64 block, u64 argument)
{
	m_selected = true;

	m_selected_collection = m_collections[collection];
	m_selected_stack	  = m_collections[collection]->get_stacks()[stack];
	m_selected_block	  = m_collections[collection]->get_stacks()[stack]->get_blocks()[block];
	m_selected_argument =
		&(m_collections[collection]->get_stacks()[stack]->get_blocks()[block]->get_arguments()[argument]);
}

void rhr::stack::plane::select_context(u64 collection, u64 stack, u64 block)
{
	m_selected = true;

	m_selected_collection = m_collections[collection];
	m_selected_stack	  = m_collections[collection]->get_stacks()[stack];
	m_selected_block	  = m_collections[collection]->get_stacks()[stack]->get_blocks()[block];
	m_selected_argument = nullptr;
}

void rhr::stack::plane::unselect()
{
	m_selected = false;

	m_selected_collection.reset();
	m_selected_stack.reset();
	m_selected_block.reset();
	m_selected_argument = nullptr;
}

void rhr::stack::plane::drag_collection(std::shared_ptr<rhr::stack::collection> collection, bool up)
{
	if (m_dragging_stack != nullptr || m_dragging_collection != nullptr)
		return;

	m_dragging_begin_object = m_dragging_collection->get_position_local_physical();

	m_dragging_up		  = up;
	m_dragging_collection = collection;
	m_dragging_stack.reset();
	m_dragging_begin_mouse = rhr::handler::input::get_mouse_position();
}

void rhr::stack::plane::drag_stack(
	std::shared_ptr<rhr::stack::collection> collection, std::shared_ptr<rhr::stack::stack> stack, bool up)
{
	if (m_dragging_stack != nullptr || m_dragging_collection != nullptr)
		return;

	// TODO: get position virtual absolute isnt working how i expected it to
	// auto virtual_absolute = collection->get_position_parent_virtual_offset() +
	// collection->get_position_local_physical() + stack->get_position_local_physical();

	auto virtual_absolute = collection->get_position_virtual_absolute() + *get_static_offset();

	stack->set_position_parent_physical({}, false);
	stack->set_position_local_physical({}, false);
	stack->set_position_parent_virtual_offset({}, false);
	stack->set_position_local_virtual_offset({}, true);

	collection->set_position_parent_physical({}, false);
	collection->set_position_local_physical({}, false);
	collection->set_position_parent_virtual_offset({}, false);
	collection->set_position_local_virtual_offset({}, true);

	//collection->set_static_offset(nullptr);

	// LOG_DEBUG_VEC2(virtual_absolute);

	m_dragging_begin_object = virtual_absolute - *get_static_offset();
	m_dragging_up			= up;
	m_dragging_begin_mouse	= rhr::handler::input::get_mouse_position();
	m_dragging_collection	= collection;
	m_dragging_stack		= stack;
}

void rhr::stack::plane::undrag(const glm::vec<2, i32>& position)
{
	if (dragging_stack())
	{
		glm::vec<2, i32> pixel_position = m_dragging_collection->get_position_virtual_absolute() + *get_static_offset();
		// LOG_DEBUG(std::to_string(pixel_position.x) + ", " + std::to_string(pixel_position.y));
		// pixel_position += *get_static_offset();

		const glm::vec<2, i32>& plane_primary_position =
			rhr::stack::plane::primary_plane->get_position_virtual_absolute();
		const glm::vec<2, i32>& plane_primary_size = rhr::stack::plane::primary_plane->get_size_local();

		const glm::vec<2, i32>& plane_toolbar_position =
			rhr::stack::plane::toolbar_plane->get_position_virtual_absolute();
		const glm::vec<2, i32>& plane_toolbar_size = rhr::stack::plane::toolbar_plane->get_size_local();

		bool over_toolbar = pixel_position.x >= plane_toolbar_position.x
			&& pixel_position.x < plane_toolbar_size.x + plane_toolbar_position.x
			&& pixel_position.y >= plane_toolbar_position.y
			&& pixel_position.y < plane_toolbar_size.y + plane_toolbar_position.y;

		bool over_primary = pixel_position.x >= plane_primary_position.x
			&& pixel_position.x < plane_primary_size.x + plane_primary_position.x
			&& pixel_position.y >= plane_primary_position.y
			&& pixel_position.y < plane_primary_size.y + plane_primary_position.y;

		// pixel_position -= *get_static_offset();

		//		if (over_toolbar)
		//			latte::logger::info("over toolbar");
		//		else
		//			latte::logger::info("NOT over toolbar");
		//
		//		if (over_primary)
		//			latte::logger::info("over primary");
		//		else
		//			latte::logger::info("NOT over primary");

		if (is_snap())
		{
			if (!over_toolbar)
			{
				if (auto dragging_snap_stack = m_dragging_snap_stack.lock())
					dragging_snap_stack->insert_blocks(m_dragging_stack->get_blocks(), m_dragging_snap_stack_loc);
				else
				{
					m_dragging_up = false;
					m_dragging_collection.reset();
					m_dragging_stack.reset();

					clear_snap();
					return;
				}

				if (auto collection = m_dragging_snap_collection.lock())
					collection->check_bounds();
				else
				{
					m_dragging_up = false;
					m_dragging_collection.reset();
					m_dragging_stack.reset();

					clear_snap();
					return;
				}

				m_dragging_collection->set_static_offset(rhr::stack::plane::primary_plane->get_static_offset());
			}
		}
		else
		{
			if (over_primary)
			{
				bool found = false;

				for (const auto& i : rhr::stack::plane::primary_plane->get_collections())
				{
					glm::vec<2, i32> collectionPosition = i->get_position_virtual_absolute() + *i->get_static_offset();
					glm::vec<2, i32> collectionSize		= i->get_size_local();

					if (pixel_position.x > collectionPosition.x
						&& pixel_position.x < collectionPosition.x + collectionSize.x
						&& pixel_position.y > collectionPosition.y
						&& pixel_position.y < collectionPosition.y + collectionSize.y)
					{
						m_dragging_stack->set_position_local_physical(pixel_position - collectionPosition, true);
						i->add_stack(m_dragging_stack);

						found = true;
						break;
					}
				}

				if (!found)
				{
					rhr::stack::plane::primary_plane->add_collection(m_dragging_collection, true);
					m_dragging_collection->set_position_local_physical(
						pixel_position - plane_primary_position - *rhr::stack::plane::primary_plane->get_static_offset(),
						true);
					m_dragging_collection->size_to_stacks(false, true);
				}

				m_dragging_collection->set_static_offset(rhr::stack::plane::primary_plane->get_static_offset());
			}
			else
			{
				// TODO: undo support (or recently deleted)
				latte::logger::warn(latte::logger::level::EDITOR, "letting stack deallocate; undo not supported yet");
			}
		}
	}

	m_dragging_up = false;
	m_dragging_collection.reset();
	m_dragging_stack.reset();

	clear_snap();
}

void rhr::stack::plane::dragging_stack_update()
{
	clear_snap();

	// pixel_position += *get_static_offset();

	m_dragging_collection->set_position_local_physical(
		rhr::handler::input::get_mouse_position() - m_dragging_begin_mouse + m_dragging_begin_object, true);
	m_dragging_connecting_line->set_position_local_physical(
		rhr::handler::input::get_mouse_position() - m_dragging_begin_mouse + m_dragging_begin_object, true);

	glm::vec<2, i32> pixel_position = m_dragging_collection->get_position_physical_absolute() + *get_static_offset();

	if (!(pixel_position.x >= rhr::stack::plane::primary_plane->get_position_virtual_absolute().x
		  && pixel_position.x < rhr::stack::plane::primary_plane->get_size_local().x
				  + rhr::stack::plane::primary_plane->get_position_virtual_absolute().x
		  && pixel_position.y >= rhr::stack::plane::primary_plane->get_position_virtual_absolute().y
		  && pixel_position.y < rhr::stack::plane::primary_plane->get_size_local().y
				  + rhr::stack::plane::primary_plane->get_position_virtual_absolute().y))
		return;

	// pixel_position -= *rhr::stack::plane::primary_plane->get_offset();

	std::shared_ptr<rhr::stack::plane> use_plane								= rhr::stack::plane::primary_plane;
	const std::vector<std::shared_ptr<rhr::stack::collection>>& use_collections = use_plane->get_collections();

	if (use_collections.empty())
		return;

	for (i64 ii = static_cast<i64>(use_collections.size()) - 1; ii >= 0; ii--)
	{
		u64 i = static_cast<u64>(ii);

		glm::vec<2, i32> collection_size = use_collections[i]->get_size_local();
		glm::vec<2, i32> collection_position =
			use_collections[i]->get_position_virtual_absolute() + *use_collections[i]->get_static_offset();

		collection_position -= glm::vec<2, i32>(SNAP_DISTANCE, SNAP_DISTANCE);
		collection_size += glm::vec<2, i32>(SNAP_DISTANCE, SNAP_DISTANCE) * 2;

		if (pixel_position.x > collection_position.x && pixel_position.x < collection_position.x + collection_size.x
			&& pixel_position.y > collection_position.y && pixel_position.y < collection_position.y + collection_size.y)
		{
			for (i64 aa = static_cast<i64>(use_collections[i]->get_stacks().size()) - 1; aa >= 0; aa--)
			{
				u64 a = static_cast<u64>(aa);

				glm::vec<2, i32> stack_size		= use_collections[i]->get_stacks()[a]->get_size_local();
				glm::vec<2, i32> stack_position = use_collections[i]->get_stacks()[a]->get_position_virtual_absolute()
					+ *use_collections[i]->get_stacks()[a]->get_static_offset();

				stack_position -= glm::vec<2, i32>(SNAP_DISTANCE, SNAP_DISTANCE);
				stack_size += glm::vec<2, i32>(SNAP_DISTANCE, SNAP_DISTANCE) * 2;

				if (pixel_position.x > stack_position.x && pixel_position.x < stack_position.x + stack_size.x
					&& pixel_position.y > stack_position.y && pixel_position.y < stack_position.y + stack_size.y)
				{
					for (u64 b = 0; b < use_collections[i]->get_stacks()[a]->get_blocks().size() + 1; b++)
					{
						u64 refIdx = 0;

						if (b > 0)
							refIdx = b - 1;

						glm::vec<2, i32> bounding_size = {
							use_collections[i]->get_stacks()[a]->get_blocks()[refIdx]->get_width()
								+ (SNAP_DISTANCE * 2),
							BLOCK_HEIGHT};
						glm::vec<2, i32> bounding_pos =
							use_collections[i]->get_stacks()[a]->get_position_virtual_absolute()
							+ *use_collections[i]->get_stacks()[a]->get_static_offset();

						bounding_pos.x -= SNAP_DISTANCE;

						bounding_pos.y += static_cast<i32>(BLOCK_HEIGHT) * static_cast<i32>(b);
						bounding_pos.y -= static_cast<i32>(static_cast<f32>(BLOCK_HEIGHT) / 2.0f);

						if (pixel_position.x >= bounding_pos.x && pixel_position.x < bounding_pos.x + bounding_size.x
							&& pixel_position.y >= bounding_pos.y
							&& pixel_position.y < bounding_pos.y + bounding_size.y)
						{
							set_snap(use_collections[i], b, use_collections[i]->get_stacks()[a]);

							return;
						}
					}

					// if stack was bounded

					break;
				}
			}

			// if collection was bounded

			break;
		}
	}
}

bool rhr::stack::plane::dragging_collection()
{
	return m_dragging_collection != nullptr && m_dragging_stack == nullptr;
}

bool rhr::stack::plane::dragging_stack()
{
	return m_dragging_collection != nullptr && m_dragging_stack != nullptr;
}

void rhr::stack::plane::set_snap(
	std::weak_ptr<rhr::stack::collection> collection, u64 stackLoc, std::weak_ptr<rhr::stack::stack> stack)
{
	m_dragging_snap = true;

	m_dragging_snap_collection = std::move(collection);
	m_dragging_snap_stack_loc  = stackLoc;
	m_dragging_snap_stack	   = stack;

	if (dragging_stack())
	{
		glm::vec<2, i32>* plane_offset = nullptr;

		if (auto lk_collection = m_dragging_snap_collection.lock())
			plane_offset = lk_collection->get_static_offset();
		else
			latte::logger::fatal(
				latte::logger::level::EDITOR, __FILE__, __LINE__, "failed to lock dragging snap collection");

		m_dragging_connecting_line->set_point_1(m_dragging_collection->get_position_physical_absolute() + *plane_offset);

		if (auto dragging_stack = stack.lock())
			m_dragging_connecting_line->set_point_2(
				dragging_stack->get_position_virtual_absolute()
				+ glm::vec<2, i32>(0, stackLoc * BLOCK_HEIGHT) + *plane_offset);
		else
			return;
	}
}

void rhr::stack::plane::clear_snap()
{
	m_dragging_snap = false;

	m_dragging_snap_collection.reset();
	m_dragging_snap_stack_loc = 0;
	m_dragging_snap_stack.reset();
}

bool rhr::stack::plane::is_snap()
{
	return m_dragging_snap;
}
