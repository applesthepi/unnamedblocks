cmake_minimum_required(VERSION 3.10)
project(UB_ESPRESSO)

set(UB_ESPRESSO_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Espresso/catagory/ModCatagory.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Espresso/mod/ModData.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Espresso/InputHandler.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Espresso/Global.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Espresso/util.cpp"
	# Headers
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Espresso/Espresso.h"
	
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Espresso/catagory/ModCatagory.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Espresso/mod/ModData.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Espresso/InputHandler.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Espresso/Global.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Espresso/util.h"
)

# Preserve folders in visual studio structure
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/Espresso FILES ${UB_ESPRESSO_SRC})

add_library(UB_ESPRESSO STATIC
	${UB_ESPRESSO_SRC}
)

add_dependencies(UB_ESPRESSO
	UB_CAPPUCCINO
)

target_include_directories(UB_ESPRESSO PUBLIC
	"${PROJECT_BINARY_DIR}"
	"${CMAKE_SOURCE_DIR}/UB_Cappuccino/include"
	"${CMAKE_CURRENT_SOURCE_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/UB_Espresso"
)

target_link_libraries(UB_ESPRESSO PUBLIC
	UB_CAPPUCCINO
)

configure_file(config.h.in config.h)