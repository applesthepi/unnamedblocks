cmake_minimum_required(VERSION 3.10)

option(BUILD_MOD_VIN "Build ModVin, which includes standard but not required blocks. If unsure keep ON" ON)
if(BUILD_MOD_VIN)
	add_subdirectory(ub-ModVin)
endif()

