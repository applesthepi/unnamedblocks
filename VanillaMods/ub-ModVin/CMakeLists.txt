cmake_minimum_required(VERSION 3.14)

project(ModVin VERSION 0.8)

add_library(ModVin MODULE "${CMAKE_CURRENT_SOURCE_DIR}/Mod.cpp")

target_include_directories(ModVin PUBLIC
	"RHR"
	"RHR/include"
	"Espresso/include"
	)
target_link_libraries(ModVin PUBLIC
	sfml-graphics
	sfml-system
	RHR
	Espresso
	)

