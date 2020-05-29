cmake_minimum_required(VERSION 3.10)
project(Cappuccino)

configure_file(config.h.in config.h)
set(Cappuccino_SRC
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/block/ModBlockActions.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/block/ModBlockFlags.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/block/ModBlockFlagsIterator.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/block/ModBlock.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/Cappuccino.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/ExecutionThread.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/ModBlockPass.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/Registration.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/runtime/ModBlockData.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/runtime/DataRegistry.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/Logger.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/runtime/DataKey.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/PreProcessorData.cpp"
	# Headers
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/block/ModBlockActions.h"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/block/ModBlockFlags.h"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/block/ModBlockFlagsIterator.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/block/ModBlock.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/Cappuccino.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/CappuccinoMod.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/ExecutionThread.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/ModBlockPass.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/Registration.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/runtime/ModBlockData.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/runtime/DataRegistry.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/Logger.h"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/runtime/DataKey.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/PreProcessorData.h"
	)
add_library(Cappuccino SHARED
	${Cappuccino_SRC}
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino FILES ${Cappuccino_SRC})
target_link_libraries(Cappuccino PUBLIC
	optimized "${csfml-audio}"
    optimized "${csfml-network}"
    optimized "${csfml-graphics}"
    optimized "${csfml-system}"
    optimized "${csfml-window}"
	debug "${csfml-audio-d}"
    debug "${csfml-network-d}"
    debug "${csfml-graphics-d}"
    debug "${csfml-system-d}"
    debug "${csfml-window-d}"
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
install(TARGETS Cappuccino DESTINATION lib)
