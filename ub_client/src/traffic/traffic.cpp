#include "traffic.hpp"

#include "glm/ext/matrix_transform.hpp"
#include "shape_lane.hpp"

constexpr u8 simulation_states_in_flight = 3;
constexpr u8 future_states_in_flight = 3;

constexpr u8 max_simulation_threads = 2;
constexpr u8 max_future_threads = 2;
constexpr u8 min_future_ops_per_thread = 50;

constexpr i32 min_simulation_poll = 100;
constexpr i32 max_simulation_poll = 1000;
#if 0
void traffic::thread_simulation_handler()
{
	std::vector<std::thread> threads;
	std::vector<sim_thread_data> data;
}

void traffic::thread_simulation()
{

}

void traffic::thread_future_handler(simulation_state* simulation_states, std::atomic_uint8_t* kill)
{
	u8 simulation_state_idx = 0;

	std::vector<std::thread> threads;
	std::vector<future_thread_data> thread_data_cache;
	std::vector<std::atomic<future_thread_data*>> thread_data;
	std::vector<bool> local_thread_standby;
	std::vector<std::atomic_uint8_t> thread_kill;
	std::vector<std::atomic_uint8_t> thread_standby;

	threads.reserve(max_future_threads);

	thread_data_cache.resize(max_future_threads * 2);
	thread_data.resize(max_future_threads);
	local_thread_standby.resize(max_future_threads);
	thread_kill.resize(max_future_threads);
	thread_standby.resize(max_future_threads);

	for (u8 i = 0; i < max_future_threads * 2; i++)
	{
		thread_data_cache[i].simulation_state = nullptr;
		thread_data_cache[i].begin = 0;
		thread_data_cache[i].end = 0;
		thread_data_cache[i].delta_time = 0.0f;
	}

	for (u8 i = 0; i < max_future_threads; i++)
	{
		thread_kill[i].store(1, std::memory_order_seq_cst);
		thread_standby[i].store(0, std::memory_order_seq_cst);
		thread_data[i].store(&thread_data_cache[i], std::memory_order_seq_cst);

		local_thread_standby[i] = false;
		threads.emplace_back(std::thread(thread_future, &thread_data[i], &thread_kill[i], &thread_standby[i]));
	}

	auto function_acquire_next_simulation = [&simulation_state_idx](u8& next_simulation_state_idx, simulation_state& current_simulation, simulation_state& next_simulation)
	{
		next_simulation.resolved.store(0, std::memory_order_release); // assume control
		current_simulation.fresh.store(1, std::memory_order_release); // release current

		simulation_state_idx = next_simulation_state_idx;
	};

	auto function_poll_simulation = [simulation_states, &simulation_state_idx, &function_acquire_next_simulation](bool force, TIME_POINT& last_now)
	{
		u8 next_simulation_state_idx = (simulation_state_idx + 1) % simulation_states_in_flight;
		auto& current_simulation = simulation_states[simulation_state_idx];
		auto& next_simulation = simulation_states[next_simulation_state_idx];

		if (force)
		{
			while (next_simulation.resolved.load(std::memory_order_acquire) == 0)
			{
				TIME_POINT waiting_now = TIME_POINT_NOW;
				std::this_thread::yield();

				if (std::chrono::duration_cast<std::chrono::milliseconds>(waiting_now - last_now).count() > max_simulation_poll)
				{
					latte::logger::error(latte::logger::level::SYSTEM, "max_simulation_poll hit");
					return;
				}
			}

			function_acquire_next_simulation(next_simulation_state_idx, current_simulation, next_simulation);
		}
		else if (next_simulation.resolved.load(std::memory_order_acquire) == 1)
			function_acquire_next_simulation(next_simulation_state_idx, current_simulation, next_simulation);
	};

	TIME_POINT last_simulation_poll = TIME_POINT_NOW;
	TIME_POINT now;

	while (kill->load(std::memory_order_seq_cst) == 0)
	{
		now = TIME_POINT_NOW;

		if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_simulation_poll).count() > min_simulation_poll)
		{
			last_simulation_poll = now;
			function_poll_simulation(false, now);
		}

		//
		// SETUP WORKER THREADS
		//

		auto& simulation = simulation_states[simulation_state_idx];

		u32 count_per_thread = static_cast<u32>(simulation.vehicles.size()) / max_future_threads;
		u32 sum_over_threads = 0;

		if (count_per_thread >= static_cast<u32>(min_future_ops_per_thread))
		{
			// RANGE DATA

			for (u8 i = 0; i < max_future_threads; i++)
			{
				auto& new_thread_data = thread_data_cache[i + max_future_threads];

				new_thread_data.simulation_state = &simulation;
				new_thread_data.begin = sum_over_threads;
				sum_over_threads += count_per_thread;
				new_thread_data.end   = sum_over_threads - 1;

				thread_data[i].store(&new_thread_data, std::memory_order_seq_cst);
			}

			u32 diff = static_cast<u32>(simulation.changed_vehicles.size()) - sum_over_threads;
			thread_data.back().end.fetch_add(diff, std::memory_order_seq_cst);

			// RELEASE

			for (u8 i = 0; i < max_future_threads; i++)
				thread_data[i].standby.store(0, std::memory_order_release);

			/// SYNCHRONIZATION

			bool all_done = false;

			while (!all_done && kill->load(std::memory_order_seq_cst) == 0)
			{
				std::this_thread::yield();

				all_done = true;
				for (u8 i = 0; i < max_future_threads; i++)
				{
					if (!local_thread_standby[i])
					{
						bool thread_done = thread_data.front().standby.load(std::memory_order_acquire) == 1;
						if (!thread_done)
							all_done = false;

						local_thread_standby[i] = thread_done;
					}
				}
			}
		}
		else
		{
			// RANGE DATA

			thread_data.front().begin.store(0, std::memory_order_seq_cst);
			thread_data.front().end.store(static_cast<u32>(simulation.changed_vehicles.size()), std::memory_order_seq_cst);

			// RELEASE

			thread_data.front().standby.store(0, std::memory_order_release);

			// SYNCHRONIZATION

			while (thread_data.front().standby.load(std::memory_order_acquire) == 0 && kill->load(std::memory_order_seq_cst) == 0)
				std::this_thread::yield();
		}
	}

	//
	// SHUTDOWN
	//

	for (u8 i = 0; i < max_future_threads; i++)
		thread_data[i].kill.store(1, std::memory_order_seq_cst);

	for (u8 i = 0; i < max_future_threads; i++)
	{
		if (threads[i].joinable())
			threads[i].join();
	}
}

void traffic::thread_future(
	std::atomic<future_thread_data*>* thread_data,
	std::atomic_uint8_t* standby,
	std::atomic_uint8_t* kill
) {
	std::vector<glm::vec<2, f32>> vehicle_positions;
	vehicle_positions.reserve(1000);

	TIME_POINT now = TIME_POINT_NOW;
	future_thread_data local_thread_data;

	while (kill->load(std::memory_order_seq_cst) == 0)
	{
		vehicle_positions.clear();

		while (standby->load(std::memory_order_seq_cst) == 1 && kill->load(std::memory_order_seq_cst) == 0)
			std::this_thread::yield();

		local_thread_data = thread_data->load(std::memory_order_acquire);
		vehicle_positions.resize(local_thread_data.simulation_state->vehicles.size());

		now = TIME_POINT_NOW;

		i32 sim_delta_ms = static_cast<i32>(std::chrono::duration_cast<std::chrono::milliseconds>(
			local_thread_data.simulation_state->resolved_time - local_thread_data.last_simulation_resolved_time
		).count());

		i32 now_delta_ms = static_cast<i32>(std::chrono::duration_cast<std::chrono::milliseconds>(
			now - local_thread_data.simulation_state->resolved_time
		).count());

		f32 future_t = static_cast<f32>(now_delta_ms) / static_cast<f32>(sim_delta_ms);
		future_t = glm::clamp(future_t, 0.0f, 1.0f);
		u8 idx = glm::clamp(
			static_cast<u8>(future_t * static_cast<f32>(local_thread_data.simulation_state->vehicle_futures_calculated)),
			static_cast<u8>(0), local_thread_data.simulation_state->vehicle_futures_calculated
		);

		if (idx == 0)
		{
			for (u32 i = local_thread_data.begin; i < local_thread_data.end; i++)
			{
				auto& vehicle = local_thread_data.simulation_state->vehicles[i];
				vehicle.speed += vehicle.acceleration * local_thread_data.delta_time;
				vehicle_positions[i] = glm::mix(vehicle.base_point, vehicle.future_points[0], future_t);
			}
		}
		else
		{
			for (u32 i = local_thread_data.begin; i < local_thread_data.end; i++)
			{
				auto& vehicle = local_thread_data.simulation_state->vehicles[i];
				vehicle.speed += vehicle.acceleration * local_thread_data.delta_time;
				vehicle_positions[i] = glm::mix(vehicle.future_points[idx - 1], vehicle.future_points[idx], future_t);
			}
		}
	}
}
#endif
void traffic::run()
{
	latte::logger::initialize();
	mac::window::global_initialization();

	auto window_state = mac::window::create("traffic", { 1920, 1080 });

	auto rectangle1 = new mac::shape_rectangle(window_state);
	rectangle1->position = { 100, 100 };
	rectangle1->color = { 1.0f, 0.5f, 1.0f, 1.0f };
	rectangle1->update();

#if 1
	{
		glm::mat4 rot = glm::lookAt(
			glm::vec<3, f32>(0.0f),
			glm::normalize(glm::vec<3, f32>(1.0f, 5.0f, 0.0f)),
			{ 0.0f, 0.0f, -1.0f }
		);

		glm::mat4 trans_base  = glm::translate(glm::mat4(1.0f), { 100.0f, 0.0f, 100.0f });
		glm::mat4 trans_width = glm::translate(glm::mat4(1.0f), { -10.0f, 0.0f, 0.0f });

		glm::vec<4, f32> v4_offset = trans_base * rot * trans_width * glm::vec<4, f32>(0.0f, 0.0f, 0.0f, 1.0f);
		glm::vec<2, f32> v_offset = { v4_offset.z, v4_offset.x };

		LOG_DEBUG_VEC2(v_offset);
		return;
	}
#endif
#if 0
	auto* lanes = new lane_sim[3];
	auto* clipsets = new clip_set[4];

	create_lane(
		lanes + 0,
		1.0f,
		1.0f
	);
	
	create_lane(
		lanes + 1,
		1.0f,
		1.0f
	);
	
	create_lane(
		lanes + 2,
		80.0f, 80.0f
	);

	create_clip_set(
		window_state,
		clipsets + 2,
		{},
		{},
		{},
		{},
		{},
		{ 0.0f, 100.0f },
		glm::normalize(glm::vec<2, f32>(0.0f, 1.0f)),
		20.0f
	);
	
	create_clip_set(
		window_state,
		clipsets + 3,
		{},
		{},
		{},
		{},
		{},
		{ 120.0f, 100.0f },
		glm::normalize(glm::vec<2, f32>(1.0f, 0.0f)),
		20.0f
	);

	create_clip_set(
		window_state,
		clipsets + 1,
		{ lanes + 1 },
		{ clipsets + 2 },
		{ 0 },
		{
			{ 1, 0, 0, 0, 0 }
		},
		{ clipsets + 2 },
		{ 0.0f, 50.0f },
		glm::normalize(glm::vec<2, f32>(0.0f, 1.0f)),
		20.0f
	);
	
	create_clip_set(
		window_state,
		clipsets + 0,
		{ lanes + 0, lanes + 2 },
		{ clipsets + 1, clipsets + 3 },
		{ 0, 0 },
		{
			{ 1, 0, 0, 0, 0 },
			{ 1, 1, 1, 0, 0 }
		},
		{ clipsets + 1, clipsets + 3 },
		{ 0.0f, 0.0f },
		glm::normalize(glm::vec<2, f32>(0.0f, 1.0f)),
		20.0f
	);
#else
	auto* lanes = new lane_sim[4];
	auto* clipsets = new clip_set[4];

	create_lane(
		lanes + 0,
		70.0f, 70.0f
	);

	create_lane(
		lanes + 1,
		70.0f, 70.0f
	);

	create_lane(
		lanes + 2,
		70.0f, 70.0f
	);

	create_lane(
		lanes + 3,
		70.0f, 70.0f
	);

	create_clip_set(
		window_state,
		clipsets + 0,
		{ lanes + 0 },
		{ clipsets + 1 },
		{ 0 },
		{
			{ 1, 0, 0, 0, 0 }
		},
		{ clipsets + 1 },
		{ 170.0f, 100.0f },
		glm::normalize(glm::vec<2, f32>(1.0f, 0.0f)),
		20.0f
	);

	create_clip_set(
		window_state,
		clipsets + 1,
		{ lanes + 1 },
		{ clipsets + 2 },
		{ 0 },
		{
			{ 1, 0, 0, 0, 0 }
		},
		{ clipsets + 2 },
		{ 240.0f, 170.0f },
		glm::normalize(glm::vec<2, f32>(0.0f, -1.0f)),
		20.0f
	);

	create_clip_set(
		window_state,
		clipsets + 2,
		{ lanes + 2 },
		{ clipsets + 3 },
		{ 0 },
		{
			{ 1, 0, 0, 0, 0 }
		},
		{ clipsets + 3 },
		{ 170.0f, 240.0f },
		glm::normalize(glm::vec<2, f32>(-1.0f, 0.0f)),
		20.0f
	);

	create_clip_set(
		window_state,
		clipsets + 3,
		{ lanes + 3 },
		{ clipsets + 0 },
		{ 0 },
		{
			{ 1, 0, 0, 0, 0 }
		},
		{ clipsets + 0 },
		{ 100.0f, 170.0f },
		glm::normalize(glm::vec<2, f32>(0.0f, 1.0f)),
		20.0f
	);

	generate_band_shapes(window_state, clipsets + 0);
	//generate_band_shapes(window_state, clipsets + 1);
	//generate_band_shapes(window_state, clipsets + 2);
	//generate_band_shapes(window_state, clipsets + 3);
#endif
	auto network_ro = new network {
		.spawn_clipsets = { clipsets + 0 }
	};



#if 0
	u8 simulation_state_idx = 0;
	auto simulation_states = new simulation_state[max_simulation_states_in_flight];

	for (u8 i = 0; i < max_simulation_states_in_flight; i++)
	{
		auto& state = simulation_states[i];

		state.network_ro = network_ro;
		state.fresh = true;
		state.resolved = false;
	}

	std::atomic_bool simulation_waiting = false;
	auto thread_inst_simulation_handler = std::thread(thread_simulation_handler, &simulation_waiting);
	thread_inst_simulation_handler.detach();
#endif

#if 0
	mac::window::add_frame_update(window_state, [
		simulation_states, &simulation_state_idx
		](f64 delta_time)
	{
		static TIME_POINT last_simulation_poll = TIME_POINT_NOW;
		TIME_POINT now = TIME_POINT_NOW;


	});
#endif

	mac::window::run(window_state);

	//if (thread_inst_simulation_handler.joinable())
	//	thread_inst_simulation_handler.join();

	mac::window::global_shutdown();
}

void traffic::create_lane(
	lane_sim* out,
	f32 p2,
	f32 p3)
{
	out->p2 = p2;
	out->p3 = p3;
}

void traffic::create_clip_set(
	mac::window::state* window_state,
	clip_set* out,
	const std::vector<lane_sim*>& lane_sims,
	const std::vector<clip_set*>& lane_future_clip_sets,
	const std::vector<u8>& lane_future_clip_set_indices,
	const std::vector<band>& bands,
	const std::vector<clip_set*>& band_future_clipsets,
	glm::vec<2, f32> position,
	glm::vec<2, f32> direction,
	f32 lane_width)
{
	out->lane_sims = lane_sims;
	out->lane_future_clip_set = lane_future_clip_sets;
	out->lane_future_clip_set_index = lane_future_clip_set_indices;
	out->bands = bands;
	out->band_future_clipsets = band_future_clipsets;

	out->position = position;
	out->direction = direction;

	out->lane_width = lane_width;
}

void traffic::generate_band_shapes(
	mac::window::state* window_state,
	clip_set* clipset
)
{
	if (clipset->bands.empty())
		return;

	glm::mat4 m_rot_base = glm::lookAt(
		glm::vec<3, f32>(0.0f),
		{ clipset->direction.x, clipset->direction.y, 0.0f },
		{ 0.0f, 0.0f, 1.0f }
	);

	glm::mat4 m_trans_base = glm::translate(glm::mat4(1.0f), {
		clipset->position.x, 0.0f, clipset->position.y
	});

	glm::mat4 m_trans_p1, m_trans_p2, m_trans_p3, m_trans_p4;
	glm::mat4 m_rot_clip, m_trans_clip;

	for (u16 i = 0; i < clipset->bands.size(); i++)
	{
		auto lane = clipset->lane_sims[clipset->bands[i].src_min];
		auto clip = clipset->band_future_clipsets[i];

		// BAND FUTURE CLIPSET

		m_rot_clip = glm::lookAt(
			glm::vec<3, f32>(0.0f),
			{ clip->direction.x, clip->direction.y, 0.0f },
			{ 0.0f, 0.0f, 1.0f }
		);

		m_trans_clip = glm::translate(glm::mat4(1.0f), {
			clip->position.x, 0.0f, clip->position.y
		});

		// BAND CONTROL POINTS

		m_trans_p1 = glm::translate(glm::mat4(1.0f), {
			clipset->lane_width * clipset->bands[i].src_min,
			0.0f, 0.0f
		});

		m_trans_p2 = glm::translate(glm::mat4(1.0f), {
			clipset->lane_width * clipset->bands[i].src_min,
			-lane->p2, 0.0f
		});

		m_trans_p3 = glm::translate(glm::mat4(1.0f), {
			clipset->lane_width * clipset->bands[i].dst_min,
			-lane->p3, 0.0f
		});

		m_trans_p4 = glm::translate(glm::mat4(1.0f), {
			clipset->lane_width * clipset->bands[i].dst_min,
			0.0f, 0.0f
		});

		glm::vec<4, f32> v4_band_p1 = m_trans_base * m_rot_base * m_trans_p1 * glm::vec<4, f32>(0.0f, 0.0f, 0.0f, 1.0f);
		glm::vec<4, f32> v4_band_p2 = m_trans_base * m_rot_base * m_trans_p2 * glm::vec<4, f32>(0.0f, 0.0f, 0.0f, 1.0f);
		glm::vec<4, f32> v4_band_p3 = m_trans_clip * m_rot_clip * m_trans_p3 * glm::vec<4, f32>(0.0f, 0.0f, 0.0f, 1.0f);
		glm::vec<4, f32> v4_band_p4 = m_trans_clip * m_rot_clip * m_trans_p4 * glm::vec<4, f32>(0.0f, 0.0f, 0.0f, 1.0f);

		//LOG_DEBUG_VEC3(v4_band_p1);
		//LOG_DEBUG_VEC3(v4_band_p2);
		
		//LOG_DEBUG_VEC2(clip->position);
		//LOG_DEBUG_VEC2(clip->direction);

		std::vector<std::string> messages;
		
		messages.emplace_back("p1: " +
			std::to_string(v4_band_p1.x) + ", " +
			std::to_string(v4_band_p1.z));
		messages.emplace_back("p2: " +
			std::to_string(v4_band_p2.x) + ", " +
			std::to_string(v4_band_p2.z));
		messages.emplace_back("p3: " +
			std::to_string(v4_band_p3.x) + ", " +
			std::to_string(v4_band_p3.z));
		messages.emplace_back("p4: " +
			std::to_string(v4_band_p4.x) + ", " +
			std::to_string(v4_band_p4.z));
		messages.emplace_back("");

		latte::logger::info(latte::logger::level::SYSTEM, messages);

		auto shape = clipset->band_shapes.emplace_back(new shape_lane(
			window_state,
			{ v4_band_p1.x, v4_band_p1.z },
			{ v4_band_p2.x, v4_band_p2.z },
			{ v4_band_p3.x, v4_band_p3.z },
			{ v4_band_p4.x, v4_band_p4.z }
		));

		shape->update();
	}
}
