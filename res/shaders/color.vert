#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform uniform_ubo_cam
{
	mat4 view;
	mat4 projection;
}
ubo_cam;

layout(binding = 1) uniform uniform_ubo_obj
{
	mat4 model;	
	vec4 color;
}
ubo_obj;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec4 in_color;

layout(location = 0) out vec4 frag_color;

void main()
{
	gl_Position =
		ubo_cam.projection *
		ubo_cam.view *
		(ubo_obj.model *
		vec4(in_position, 1.0));
	
	frag_color = in_color * ubo_obj.color;
}