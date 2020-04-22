cmake_minimum_required(VERSION 3.10)

option(BUILD_MOD_VIN "Build ModVin, which includes standard but not required blocks. If unsure keep ON" ON)
if(BUILD_MOD_VIN)
	add_subdirectory(ub-ModVin)
	#	set_target_properties(ModVin-Dynamic
	#		PROPERTIES
	#		LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/mods"
	#		PREFIX ""
	#		)
	#	set_target_properties(ModVin-Static
	#		PROPERTIES
	#		LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/mods"
	#		PREFIX ""
	#		)
endif()

