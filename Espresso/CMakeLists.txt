cmake_minimum_required(VERSION 3.14)

# use c++ 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

project(Espresso VERSION 0.8)
find_package(SFML REQUIRED COMPONENTS graphics system)
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
	${SFML_LIBRARIES}
)
target_include_directories(Espresso PUBLIC
	"${PROJECT_BINARY_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Espresso"
	${SFML_INCLUDE_DIRS}
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
