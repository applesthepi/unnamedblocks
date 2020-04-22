cmake_minimum_required(VERSION 3.10)

project(Espresso VERSION 0.8)
configure_file(config.h.in config.h)

add_library(Espresso STATIC
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Espresso/block/ModBlock.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Espresso/block/ModBlockActions.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Espresso/block/ModBlockFlags.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Espresso/block/ModBlockFlagsIterator.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Espresso/catagory/ModCatagory.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Espresso/mod/ModData.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Espresso/preprocessor/PreProcessorData.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Espresso/Logger.cpp"
	# Headers
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Espresso/block/ModBlock.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Espresso/block/ModBlockActions.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Espresso/block/ModBlockFlags.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Espresso/block/ModBlockFlagsIterator.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Espresso/catagory/ModCatagory.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Espresso/Espresso.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Espresso/Logger.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Espresso/mod/ModData.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Espresso/preprocessor/PreProcessorData.h"
)
target_include_directories(Espresso PUBLIC
	"${PROJECT_BINARY_DIR}"
	"${CMAKE_SOURCE_DIR}/Cappuccino/include"
	"${CMAKE_CURRENT_SOURCE_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Espresso"
)
target_link_libraries(Espresso PUBLIC
	sfml-graphics
	sfml-window
	sfml-system
	Cappuccino
)

