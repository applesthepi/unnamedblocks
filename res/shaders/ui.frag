#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragColor;

layout(location = 0) out vec4 outColor;

void main()
{
	outColor = vec4(pow(fragColor.r, 2), pow(fragColor.g, 2), pow(fragColor.b, 2), 1.0);
}