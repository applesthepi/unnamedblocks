cmake_minimum_required(VERSION 3.10)
project(Cappuccino)

configure_file(config.h.in config.h)
add_library(Cappuccino STATIC
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/Cappuccino.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/ExecutionThread.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/ModBlockPass.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/Registration.cpp"
	# Headers
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/Cappuccino.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/ExecutionThread.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/ModBlockPass.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cappuccino/Registration.h"
)
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
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	"${CMAKE_SOURCE_DIR}/Espresso/include"
)
set_target_properties(Cappuccino PROPERTIES PREFIX "lib")
set_target_properties(Cappuccino PROPERTIES SUFFIX ".a")