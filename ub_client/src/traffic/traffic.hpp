#pragma once
#include "config.h"

#include "utils.hpp"

#include <latte/utils.hpp>
#include <macchiato/objects/object_color.hpp>
#include <macchiato/objects/object_texture.hpp>
#include <macchiato/vertices/vertex_color.hpp>
#include <macchiato/vertices/vertex_texture.hpp>
#include <macchiato/shapes/rectangle.hpp>
#include <macchiato/window.hpp>

namespace traffic
{
struct vehicle_sim
{
	f32 speed;
	f32 acceleration;

	glm::vec<2, f32> base_point;
	std::vector<glm::vec<2, f32>> future_points;
};

struct lane_sim
{
	glm::vec<2, f32> p2;
	glm::vec<2, f32> p3;
};

struct band
{
	u8 lane_count;
	u8 src_min;
	u8 src_max;
	u8 dst_min;
	u8 dst_max;
};

struct clip_set
{
	std::vector<lane_sim*> lane_sims;
	std::vector<vehicle_sim*> lane_cars;
	std::vector<clip_set*> lane_future_clip_set;
	std::vector<u8> lane_future_clip_set_index;
	std::vector<band> bands;

	glm::vec<2, f32> position;
	glm::vec<2, f32> direction;
};

struct vehicle_net
{
	vehicle_sim sim;

	clip_set* clip_set;
	u8 clip_set_lane_idx;
};

/// Structure of the roads, shared as read only between everything. When it gets changed, all simulation states
/// must be resolved.
struct network
{
	std::vector<clip_set*> spawn_clipsets;
	std::vector<vehicle_net*> vehicles;
};

/// Multiple simulation states can be in flight to be as consistently ready for future processing.
struct simulation_state
{
	network* network_ro;

	/// When fresh is flagged by the future handler, it no longer writes.
	std::atomic_uint8_t fresh;

	/// When resolved is flagged by the simulation handler, it no longer writes.
	std::atomic_uint8_t resolved;
	TIME_POINT resolved_time;

	// maybe dont need these
	//std::vector<u32> changed_vehicle_idx;
	u8 vehicle_futures_calculated;
	std::vector<vehicle_sim> vehicles;
	std::vector<lane_sim*> changed_vehicle_lane_sims;
};

/// Processes the same simulation state until the next one is produced. The futures are triple buffered and oldest
/// is overwritten with the spacing being the delta time of the render thread.
struct future_state
{
	std::atomic_uint8_t fresh;
	std::atomic_uint8_t resolved;
	TIME_POINT resolved_time;

	std::vector<glm::vec<2, f32>> vehicle_positions;
};

struct sim_thread_data
{

};

struct future_thread_data
{
	TIME_POINT last_simulation_resolved_time;
	simulation_state* simulation_state;
	u32 begin;
	u32 end;
	f32 delta_time;
};

void thread_simulation_handler();

void thread_simulation();

void thread_future_handler(
	simulation_state* simulation_states,
	std::atomic_uint8_t* kill
);

void thread_future(
	std::atomic<future_thread_data*>* thread_data,
	std::atomic_uint8_t* standby,
	std::atomic_uint8_t* kill
);

void run();
}