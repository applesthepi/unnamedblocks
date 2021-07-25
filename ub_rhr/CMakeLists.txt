cmake_minimum_required(VERSION 3.15)
project(UB_RHR)

set(UB_RHR_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/handlers/message.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/handlers/message.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/handlers/project.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/handlers/project.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/handlers/statment.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/handlers/statment.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/handlers/context.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/handlers/context.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/handlers/category.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/handlers/category.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/handlers/preprocessor.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/handlers/preprocessor.cpp"

	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/registries/block.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/registries/block.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/registries/char_texture.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/registries/char_texture.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/registries/shader.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/registries/shader.cpp"
	
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/plane.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/plane.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/collection.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/collection.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/stack.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/stack.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/block.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/block.cpp"
	
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/arguments/argument.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/arguments/argument.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/arguments/any.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/arguments/any.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/arguments/boolean.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/arguments/boolean.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/arguments/dropdown.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/arguments/dropdown.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/arguments/real.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/arguments/real.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/arguments/string.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/arguments/string.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/arguments/text.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/arguments/text.cpp"
	
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/rendering/renderer.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/rendering/renderer.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/rendering/frame.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/rendering/frame.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/rendering/layer.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/rendering/layer.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/rendering/tools.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/rendering/tools.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/rendering/Vertex.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/rendering/Vertex.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/rendering/objects/progress_bar.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/rendering/objects/progress_bar.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/rendering/objects/object.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/rendering/objects/object.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/rendering/objects/rectangle.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/rendering/objects/rectangle.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/rendering/objects/text.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/rendering/objects/text.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/rendering/objects/button.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/rendering/objects/button.cpp"

	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/rendering/interfaces/i_positionable.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/rendering/interfaces/i_positionable.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/rendering/interfaces/i_sizeable.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/rendering/interfaces/i_sizeable.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/rendering/interfaces/i_scalable.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/rendering/interfaces/i_scalable.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/rendering/interfaces/i_renderable.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/rendering/interfaces/i_renderable.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/rendering/interfaces/i_updateable.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/rendering/interfaces/i_updateable.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/rendering/interfaces/i_colorable.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/rendering/interfaces/i_colorable.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/rendering/interfaces/i_dicolorable.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/rendering/interfaces/i_dicolorable.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/rendering/interfaces/i_enableable.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/rendering/interfaces/i_enableable.cpp"
)

add_library(UB_RHR STATIC
	${UB_RHR_SRC}
)

if(MSVC)
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/rhr FILES ${UB_RHR_SRC})
	target_compile_options(UB_RHR PRIVATE "/MP")
endif()

add_dependencies(UB_CLIENT
    UB_CAPPUCCINO
    UB_ESPRESSO
)

target_link_libraries(UB_RHR PUBLIC
	UB_CAPPUCCINO
	UB_ESPRESSO
	
	glad::glad
	glfw
	imgui::imgui
	Vulkan::Vulkan
	freetype
)

target_include_directories(UB_RHR PUBLIC
	"${PROJECT_BINARY_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/ub_cappuccino/include"
    "${CMAKE_SOURCE_DIR}/ub_espresso/include"

    ${STB_INCLUDE_DIRS}
	${RAPIDJSON_INCLUDE_DIRS}
	${VULKAN_INCLUDE_DIRS}
)

configure_file(config.h.in config.h)