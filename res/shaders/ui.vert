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
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;

void main()
{
	vec4 model = ubo.model * vec4(inPosition, 1.0);
	vec4 view  = ubo.view * model;
	vec4 proj  = ubo.proj * view;
	
	gl_Position = proj;
	fragColor = inColor;
}