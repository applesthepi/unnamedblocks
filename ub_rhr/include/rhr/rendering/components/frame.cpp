#include "frame.hpp"

rhr::render::component::frame::frame()
	: frame_buffer(nullptr)
	, frame_image(nullptr)
	, frame_view(nullptr)
	, fence_in_flight(nullptr)
	, fence_image_in_flight(nullptr)
	, semaphore_image(nullptr)
	, semaphore_finished(nullptr)
{}

rhr::render::component::frame::~frame() {}
