#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform ubo_cam_block
{
	mat4 view;
	mat4 projection;
} ubo_cam;

layout(set = 1, binding = 0) uniform ubo_obj_block
{
	vec4 color;
	mat4 model;
} ubo_obj;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_texture_coordinates;

layout(location = 0) out vec4 frag_color;
layout(location = 1) out vec2 frag_texture_coordinates;

void main()
{
	gl_Position = 
		ubo_cam.projection *
		ubo_cam.view *
		(ubo_obj.model * vec4(in_position, 1.0));
	
	frag_color = ubo_obj.color;
	frag_texture_coordinates = in_texture_coordinates;
}