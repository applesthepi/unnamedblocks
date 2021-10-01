#include "plane.hpp"

#include "rhr/rendering/renderer.hpp"

static void primary_plane_mouse_button(glm::vec<2, i32> position, f32 scroll, MouseOperation operation, MouseButton button, void* data)
{
	rhr::stack::plane::primary_plane->mouse_button(position, scroll, operation);
}

static void toolbar_plane_mouse_button(glm::vec<2, i32> position, f32 scroll, MouseOperation operation, MouseButton button, void* data)
{
	rhr::stack::plane::toolbar_plane->mouse_button(position, scroll, operation);
}

rhr::stack::plane::plane(bool toolbar)
	: m_toolbar(toolbar)
	, m_selected(false)
	, m_selected_context(false)
	, m_dragging_collection(nullptr)
	, m_dragging_stack(nullptr)
	, m_dragging_up(false)
	, m_dragging_snap(false)
	, m_dragging_snap_collection(0)
	, m_dragging_snap_stack(0)
	, m_background(std::make_shared<rhr::render::object::rectangle>())
{
	if (toolbar)
		InputHandler::RegisterMouseCallback(toolbar_plane_mouse_button, nullptr);
	else
		InputHandler::RegisterMouseCallback(primary_plane_mouse_button, nullptr);

	m_background->set_weak(m_background);
	update_child_transform(m_background);
	m_background->set_size_local(get_size_local());
	m_background->set_color(cap::color().from_u8({ 0, 0, 0, 255 }));
	m_background->set_depth(rhr::render::renderer::depth_plane);

	m_collections.reserve(5);
	m_collection_vanity.reserve(5);

	m_context_callback = [](u8 idx)
	{
		if (idx == 0)
		{
			// stack duplicate

			//m_selectedCollection->add_stack(new rhr::stack::stack(*m_selectedStack));
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

void rhr::stack::plane::add_collection(std::shared_ptr<rhr::stack::collection>& collection, bool display_collection_vanity)
{
	update_child_transform(collection);
	m_collections.push_back(collection);
	m_collection_vanity.push_back(display_collection_vanity);

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

void rhr::stack::plane::mouse_button(glm::vec<2, i32> position, f32 scroll, MouseOperation operation)
{
	if (operation != MouseOperation::Press && operation != MouseOperation::Release)
		return;

	if ((m_toolbar && rhr::stack::plane::primary_plane->dragging_stack()) ||
		(!m_toolbar && rhr::stack::plane::toolbar_plane->dragging_stack()))
		return;

	if (dragging_stack())
	{
		if (operation == MouseOperation::Release && !m_dragging_up)
		{
			// dragging and mouse released (used when dragging a stack)
			undrag(position);
		}
		else if (operation == MouseOperation::Press && m_dragging_up)
		{
			// dragging and mouse pressed (used when duplicating stack)
			undrag(position);
		}
	}

//	cap::logger::debug("mouse pos", position);

	for (i64 i = /*collectionMax - 1*/0; i < m_collections.size(); i++)
	{
		if (!m_collections[i]->get_enabled())
			continue;

		glm::vec<2, i32> collection_size = m_collections[i]->get_size_local();
		glm::vec<2, i32> collection_position = m_collections[i]->get_position_virtual_absolute();

		if (position.x > collection_position.x && position.x < collection_position.x + collection_size.x &&
			position.y > collection_position.y && position.y < collection_position.y + collection_size.y)
		{
			for (i64 a = m_collections[i]->get_stacks().size() - 1; a >= 0; a--)
			{
				glm::vec<2, i32> stack_size = m_collections[i]->get_stacks()[a]->get_size_local();
				glm::vec<2, i32> stack_position = m_collections[i]->get_stacks()[a]->get_position_virtual_absolute();

				if (position.x > stack_position.x && position.x < stack_position.x + stack_size.x &&
					position.y > stack_position.y && position.y < stack_position.y + stack_size.y)
				{
					for (u64 b = 0; b < m_collections[i]->get_stacks()[a]->get_blocks().size(); b++)
					{
						glm::vec<2, i32> block_size = m_collections[i]->get_stacks()[a]->get_blocks()[b]->get_size_local();
						glm::vec<2, i32> block_position = m_collections[i]->get_stacks()[a]->get_blocks()[b]->get_position_virtual_absolute();

//						cap::logger::debug("virtual offset of block", m_collections[i]->get_stacks()[a]->get_blocks()[b]->get_position_virtual_offset());

						if (position.x >= block_position.x && position.x < block_position.x + block_size.x &&
							position.y >= block_position.y && position.y < block_position.y + block_size.y)
						{
							if (operation == MouseOperation::Press && !dragging_stack())
							{
								// not dragging and mouse down

								if (true/* left or right click */)
								{
									// remove context menu

									//ContextHandler::Disable();
									unselect();

									if (m_collections[i]->get_stacks()[a]->get_blocks()[b]->drag_bounds(position))
										return;

									std::shared_ptr<rhr::stack::collection> active_collection = std::make_shared<rhr::stack::collection>();
									active_collection->set_weak(active_collection);
									update_child_transform(active_collection);

									std::shared_ptr<rhr::stack::stack> active_stack = m_collections[i]->get_stacks()[a];

									if (m_toolbar)
									{
										std::shared_ptr<rhr::stack::stack> dragging_stack = std::make_shared<rhr::stack::stack>();
										dragging_stack->set_weak(dragging_stack);

										const std::vector<std::shared_ptr<rhr::stack::block>>& blocks = active_stack->get_blocks();
										std::vector<std::shared_ptr<rhr::stack::block>> cloned_blocks;

										for (auto block : blocks)
										{
											std::shared_ptr<rhr::stack::block> cloned_block = std::make_shared<rhr::stack::block>(block->get_mod_block()->get_unlocalized_name());
											cloned_block->set_weak(cloned_block);

											cloned_blocks.push_back(std::move(cloned_block));
										}

										dragging_stack->add_blocks(cloned_blocks);
										active_stack = dragging_stack;
									}

									if (b > 0)
									{
										if (m_toolbar)
										{
											cap::logger::error("splitting stacks inside toolbar is not supported");
											return;
										}

										// create stack that is left behind (no the one picked up)

										std::shared_ptr<rhr::stack::stack> left_stack = std::make_shared<rhr::stack::stack>();
										left_stack->set_weak(left_stack);
										left_stack->set_position_local_physical(active_stack->get_position_local_physical());

										for (u64 c = 0; c < b; c++)
											left_stack->add_block(m_collections[i]->get_stacks()[a]->get_blocks()[c]);

										for (u64 c = 0; c < b; c++)
											active_stack->remove_block(0);

										m_collections[i]->add_stack(left_stack, false);
										stack_position += glm::vec<2, i32>(0, static_cast<i32>(b) * static_cast<i32>(rhr::stack::block::height));
									}

									active_collection->set_position_parent_physical(get_position_virtual_absolute(), false);
									active_collection->set_position_local_physical(stack_position - active_collection->get_position_virtual_offset(), false);
									active_collection->set_size_local(stack_size);

									active_stack->set_position_parent_physical({ 0, 0 }, false);
									active_stack->set_position_local_physical({ 0, 0 });

									if (!m_toolbar)
										m_collections[i]->remove_stack(a);

									active_collection->add_stack(active_stack, false);
									active_collection->size_to_stacks(true, false);
									
									drag_stack(active_collection, active_stack, false);

									if (m_collections[i]->get_stacks().size() == 0)
										delete_collection(i);

									return;
								}
								else if (false/* left or right click */)
								{
									//cap::logger::Debug("opening context menu");

									// startup context menu on block

									//ContextHandler::Disable();
									//ContextHandler::Enable(mmpos + glm::vec<2, i32>(5, 5), &m_contextCallback);
									//SelectContext(i, a, b);
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
}

rhr::handler::field& rhr::stack::plane::get_field()
{
	return m_field;
}

void rhr::stack::plane::render_master_pass()
{
	if (dragging_stack() || dragging_collection())
		m_dragging_collection->render();
}

void rhr::stack::plane::ui_transform_update()
{
	set_size_max(false);
	update_child_transform(m_background);

	for (auto& collection : m_collections)
		update_child_transform(collection);
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
		m_dragging_collection->reload_swap_chain();
}

void rhr::stack::plane::ui_update_buffers()
{
	m_background->update_buffers();

	for (auto& collection : m_collections)
		collection->update_buffers();

	if (dragging_stack() || dragging_collection())
		m_dragging_collection->update_buffers();
}

void rhr::stack::plane::ui_frame_update(f64 delta_time)
{
	m_background->frame_update(delta_time);

	for (auto& collection : m_collections)
		collection->frame_update(delta_time);

	if (dragging_stack() || dragging_collection())
		m_dragging_collection->frame_update(delta_time);

	// dragging position

//	if (dragging_stack() || dragging_collection())
//	{
//		m_dragging_collection->set_position_local_physical(m_dragging_begin_object + glm::vec<2, i32>(
//			(InputHandler::GetMousePosition().x - m_dragging_begin_mouse.x),
//			(InputHandler::GetMousePosition().y - m_dragging_begin_mouse.y)) - get_position_virtual_absolute());
//	}

	// dragging connections

	if (dragging_stack())
		dragging_stack_update();
}

std::shared_ptr<rhr::stack::plane> rhr::stack::plane::primary_plane;
std::shared_ptr<rhr::stack::plane> rhr::stack::plane::toolbar_plane;

void rhr::stack::plane::select(u64 collection, u64 stack, u64 block, u64 argument)
{
	m_selected = true;

	m_selected_collection = m_collections[collection];
	m_selected_stack = m_collections[collection]->get_stacks()[stack];
	m_selected_block = m_collections[collection]->get_stacks()[stack]->get_blocks()[block];
	m_selected_argument = m_collections[collection]->get_stacks()[stack]->get_blocks()[block]->get_arguments()[argument];
}

void rhr::stack::plane::select_context(u64 collection, u64 stack, u64 block)
{
	m_selected = true;

	m_selected_collection = m_collections[collection];
	m_selected_stack = m_collections[collection]->get_stacks()[stack];
	m_selected_block = m_collections[collection]->get_stacks()[stack]->get_blocks()[block];
	m_selected_argument = nullptr;
}

void rhr::stack::plane::unselect()
{
	m_selected = false;

	m_selected_collection = nullptr;
	m_selected_stack = nullptr;
	m_selected_block = nullptr;
	m_selected_argument = nullptr;
}

void rhr::stack::plane::drag_collection(std::shared_ptr<rhr::stack::collection>& collection, bool up)
{
	if (m_dragging_stack != nullptr || m_dragging_collection != nullptr)
		return;

	m_dragging_up = up;

	m_dragging_collection = collection;
	m_dragging_stack = nullptr;

	m_dragging_begin_mouse = InputHandler::GetMousePosition();
	m_dragging_begin_object = m_dragging_collection->get_position_local_physical();
}

void rhr::stack::plane::drag_stack(std::shared_ptr<rhr::stack::collection>& collection, std::shared_ptr<rhr::stack::stack>& stack, bool up)
{
	if (m_dragging_stack != nullptr || m_dragging_collection != nullptr)
		return;

	m_dragging_up = up;

	m_dragging_begin_mouse = InputHandler::GetMousePosition();
	m_dragging_begin_object = collection->get_position_local_physical();

	m_dragging_collection = collection;
	m_dragging_stack = stack;
}

void rhr::stack::plane::undrag(const glm::vec<2, i32>& position)
{
	if (dragging_stack())
	{
		glm::vec<2, i32> pixel_position = m_dragging_collection->get_position_physical_absolute();

		const glm::vec<2, i32>& plane_primary_position = rhr::stack::plane::primary_plane->get_position_virtual_absolute();
		const glm::vec<2, i32>& plane_primary_size = rhr::stack::plane::primary_plane->get_size_local();

		const glm::vec<2, i32>& plane_toolbar_position = rhr::stack::plane::toolbar_plane->get_position_virtual_absolute();
		const glm::vec<2, i32>& plane_toolbar_size = rhr::stack::plane::toolbar_plane->get_size_local();

		bool over_toolbar =
			pixel_position.x >= plane_toolbar_position.x && pixel_position.x < plane_toolbar_size.x + plane_toolbar_position.x &&
			pixel_position.y >= plane_toolbar_position.y && pixel_position.y < plane_toolbar_size.y + plane_toolbar_position.y;

		bool over_primary =
			pixel_position.x >= plane_primary_position.x && pixel_position.x < plane_primary_size.x + plane_primary_position.x &&
			pixel_position.y >= plane_primary_position.y && pixel_position.y < plane_primary_size.y + plane_primary_position.y;

		if (over_toolbar)
			cap::logger::debug("over toolbar");
		else
			cap::logger::debug("NOT over toolbar");

		if (over_primary)
			cap::logger::debug("over primary");
		else
			cap::logger::debug("NOT over primary");

		if (is_snap())
		{
			if (!over_toolbar)
			{
				m_dragging_snap_stack->insert_blocks(m_dragging_stack->get_blocks(), m_dragging_snap_stack_loc);
				m_dragging_snap_collection->check_bounds();
			}
		}
		else
		{
			if (over_primary)
			{
				cap::logger::debug("dropping over primary plane");
				bool found = false;

				for (const auto& i : rhr::stack::plane::primary_plane->get_collections())
				{
					glm::vec<2, i32> collectionPosition = i->get_position_virtual_absolute();
					glm::vec<2, i32> collectionSize = i->get_size_local();

					if (pixel_position.x > collectionPosition.x && pixel_position.x < collectionPosition.x + collectionSize.x &&
						pixel_position.y > collectionPosition.y && pixel_position.y < collectionPosition.y + collectionSize.y)
					{
						m_dragging_stack->set_position_local_physical(pixel_position - collectionPosition);
						i->add_stack(m_dragging_stack);

						found = true;
						break;
					}
				}

				if (!found)
				{
					rhr::stack::plane::primary_plane->add_collection(m_dragging_collection, true);
					m_dragging_collection->set_position_local_physical(pixel_position - plane_primary_position);
//					m_dragging_collection->set_position_par
					m_dragging_collection->size_to_stacks(false, true);
				}
			}
			else
			{
				// TODO: undo support (or recently deleted)
				cap::logger::warn("letting stack deallocate; undo not supported yet");
			}
		}
	}

	m_dragging_up = false;
	m_dragging_collection = nullptr;
	m_dragging_stack = nullptr;

	clear_snap();
}

void rhr::stack::plane::dragging_stack_update()
{
	clear_snap();

	glm::vec<2, i32> pixel_position = m_dragging_collection->get_position_physical_absolute();

//	m_dragging_collection->set_position_parent_physical({ 0, 0 }, false);
	m_dragging_collection->set_position_local_physical(InputHandler::GetMousePosition() - m_dragging_begin_mouse + m_dragging_begin_object);

	if (!(pixel_position.x >= rhr::stack::plane::primary_plane->get_position_virtual_absolute().x && pixel_position.x < rhr::stack::plane::primary_plane->get_size_local().x + rhr::stack::plane::primary_plane->get_position_virtual_absolute().x &&
		pixel_position.y >= rhr::stack::plane::primary_plane->get_position_virtual_absolute().y && pixel_position.y < rhr::stack::plane::primary_plane->get_size_local().y + rhr::stack::plane::primary_plane->get_position_virtual_absolute().y))
		return;

	std::shared_ptr<rhr::stack::plane> use_plane = rhr::stack::plane::primary_plane;
	const std::vector<std::shared_ptr<rhr::stack::collection>>& use_collections = use_plane->get_collections();

	u64 collection_max = use_collections.size();

	if (collection_max == 0)
		return;

	for (u64 i = 0; i < collection_max; i++)
	{
		glm::vec<2, i32> collection_size = use_collections[i]->get_size_local();
		glm::vec<2, i32> collection_position = use_collections[i]->get_position_virtual_absolute();

		collection_position -= glm::vec<2, i32>(SNAP_DISTANCE, SNAP_DISTANCE);
		collection_size += glm::vec<2, i32>(SNAP_DISTANCE, SNAP_DISTANCE) * 2;

		if (pixel_position.x > collection_position.x && pixel_position.x < collection_position.x + collection_size.x &&
			pixel_position.y > collection_position.y && pixel_position.y < collection_position.y + collection_size.y)
		{
			//std::cout << "collection " << i << std::endl;

			for (i64 a = use_collections[i]->get_stacks().size() - 1; a >= 0; a--)
			{
				glm::vec<2, i32> stack_size = use_collections[i]->get_stacks()[a]->get_size_local();
				glm::vec<2, i32> stack_position = use_collections[i]->get_stacks()[a]->get_position_virtual_absolute();

				stack_position -= glm::vec<2, i32>(SNAP_DISTANCE, SNAP_DISTANCE);
				stack_size += glm::vec<2, i32>(SNAP_DISTANCE, SNAP_DISTANCE) * 2;
				
				if (pixel_position.x > stack_position.x && pixel_position.x < stack_position.x + stack_size.x &&
					pixel_position.y > stack_position.y && pixel_position.y < stack_position.y + stack_size.y)
				{
					//std::cout << "stack " << a << std::endl;

					for (u64 b = 0; b < use_collections[i]->get_stacks()[a]->get_blocks().size() + 1; b++)
					{
						u64 refIdx = 0;

						if (b > 0)
							refIdx = b - 1;

						glm::vec<2, i32> bounding_size = { use_collections[i]->get_stacks()[a]->get_blocks()[refIdx]->get_width() + (SNAP_DISTANCE * 2), rhr::stack::block::height };
						glm::vec<2, i32> bounding_pos = use_collections[i]->get_stacks()[a]->get_position_virtual_absolute();

						bounding_pos.x -= SNAP_DISTANCE;

						bounding_pos.y += static_cast<i32>(rhr::stack::block::height) * b;
						bounding_pos.y -= static_cast<f32>(rhr::stack::block::height) / 2.0f;

						if (pixel_position.x >= bounding_pos.x && pixel_position.x < bounding_pos.x + bounding_size.x &&
							pixel_position.y >= bounding_pos.y && pixel_position.y < bounding_pos.y + bounding_size.y)
						{
							//std::cout << "snapping " << b << std::endl;
							set_snap(use_collections[i], b, use_collections[i]->get_stacks()[a]);

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
}

bool rhr::stack::plane::dragging_collection()
{
	return m_dragging_collection != nullptr && m_dragging_stack == nullptr;
}

bool rhr::stack::plane::dragging_stack()
{
	return m_dragging_collection != nullptr && m_dragging_stack != nullptr;
}

void rhr::stack::plane::set_snap(std::shared_ptr<rhr::stack::collection> collection, u64 stackLoc, std::shared_ptr<rhr::stack::stack> stack)
{
	m_dragging_snap = true;

	m_dragging_snap_collection = collection;
	m_dragging_snap_stack_loc = stackLoc;
	m_dragging_snap_stack = stack;
}

void rhr::stack::plane::clear_snap()
{
	m_dragging_snap = false;

	m_dragging_snap_collection = nullptr;
	m_dragging_snap_stack_loc = 0;
	m_dragging_snap_stack = nullptr;
}

bool rhr::stack::plane::is_snap()
{
	return m_dragging_snap;
}
