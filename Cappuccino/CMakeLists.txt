cmake_minimum_required(VERSION 3.10)
project(Cappuccino)

configure_file(config.h.in config.h)
set(Cappuccino_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/Cappuccino.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/ExecutionThread.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/ModBlockPass.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/Registration.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/runtime/ModBlockData.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/runtime/DataRegistry.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/Logger.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/runtime/DataKey.cpp"
	# Headers
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/Cappuccino.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/ExecutionThread.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/ModBlockPass.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/Registration.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/runtime/ModBlockData.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/runtime/DataRegistry.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/Logger.h"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/runtime/DataKey.h"
	)
add_library(Cappuccino SHARED
	${Cappuccino_SRC}
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino FILES ${Cappuccino_SRC})
target_link_libraries(Cappuccino PUBLIC
	"${csfml-audio}"
    "${csfml-network}"
    "${csfml-graphics}"
    "${csfml-system}"
    "${csfml-window}"
)
target_include_directories(Cappuccino PUBLIC
	"${PROJECT_BINARY_DIR}"
	"${csfml_include}"
	"${CMAKE_CURRENT_SOURCE_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
)
set_target_properties(Cappuccino PROPERTIES PREFIX "lib")

add_custom_command(
	TARGET Cappuccino POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E touch
	"${CMAKE_SOURCE_DIR}/UnnamedBlocks/src/main.cpp"
)