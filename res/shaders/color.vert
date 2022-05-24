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
layout(location = 1) in vec4 in_color;

layout(location = 0) out vec4 frag_color;

void main()
{
	// gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);

	gl_Position = 
		ubo_cam.projection *
		ubo_cam.view *
		(ubo_obj.model * vec4(in_position, 1.0));

	// gl_Position =
	// 	ubo_cam.projection *
	// 	ubo_cam.view *
	// 	(ubo_obj.model *
	// 	vec4(in_position, 1.0));
	
	frag_color = in_color * ubo_obj.color;
}