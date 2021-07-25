function build_shader {
	glslc "res/shaders/$1.frag" -o "res/shaders/$1.frag.spv"
	glslc "res/shaders/$1.vert" -o "res/shaders/$1.vert.spv"
}

build_shader "ui"
