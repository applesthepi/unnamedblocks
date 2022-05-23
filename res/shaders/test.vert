#version 450
// #extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform vec3 position;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec4 in_color;

layout(location = 0) out vec4 frag_color;

void main()
{
	gl_Position = vec4(position, 1.0);
}