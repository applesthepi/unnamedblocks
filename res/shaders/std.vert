#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject
{
	mat4 model;
	mat4 view;
	mat4 proj;
	
	vec3 color;
}
ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec4 fragColor;

void main()
{
	mat4 projView  = ubo.proj * ubo.view;
	vec4 pos = projView * (ubo.model * vec4(inPosition, 1.0));
	
	gl_Position = pos;
	fragColor = inColor;
}