cmake_minimum_required(VERSION 3.10)

project(RHR)
configure_file(config.h.in config.h)
set(RHR_SRC
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
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/handlers/InputHandler.cpp"
	#${CMAKE_CURRENT_SOURCE_DIR}/ Headers
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/handlers/MessageHandler.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/Global.h"
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
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/handlers/InputHandler.h"
	)
add_library(RHR STATIC
	${RHR_SRC}
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

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/RHR FILES ${RHR_SRC})
