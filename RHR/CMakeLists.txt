cmake_minimum_required(VERSION 3.10)

# use c++ 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

project(RHR VERSION 0.8)
configure_file(config.h.in config.h)
add_library(RHR STATIC
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/Global.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/handlers/MessageHandler.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/handlers/ProjectHandler.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/handlers/runtime/PreProcessor.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/handlers/StatmentHandler.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/registries/BlockRegistry.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/registries/ButtonRegistry.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/registries/ShaderRegistry.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/args/Argument.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/Block.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/Plane.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/Stack.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/StatmentIf.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/Button.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/TypingSystem.cpp"
	# Headers
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/Global.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/handlers/MessageHandler.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/handlers/ProjectHandler.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/handlers/runtime/PreProcessor.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/handlers/StatmentHandler.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/registries/BlockRegistry.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/registries/ButtonRegistry.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/registries/ShaderRegistry.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/RHR.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/args/Argument.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/args/ArgumentBoolean.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/args/ArgumentString.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/args/ArgumentText.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/args/ArgumetReal.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/Block.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/Plane.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/Stack.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/StatmentIf.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/Button.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/TypingSystem.h"
)
target_link_libraries(RHR PUBLIC
	Espresso
	sfml-graphics
	sfml-system
	sfml-window
	${libtcc}
)
target_include_directories(RHR PUBLIC
	"${PROJECT_BINARY_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR"
	"${CMAKE_SOURCE_DIR}/Espresso/include"
	${tcc_include}
)

#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
