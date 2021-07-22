cmake_minimum_required(VERSION 3.15)
project(UB_RHR)

set(UB_RHR_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/handlers/MessageHandler.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/handlers/MessageHandler.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/handlers/ProjectHandler.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/handlers/ProjectHandler.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/handlers/StatmentHandler.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/handlers/StatmentHandler.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/handlers/ContextHandler.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/handlers/ContextHandler.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/handlers/CategoryHandler.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/handlers/CategoryHandler.cpp"

	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/handlers/runtime/PreProcessor.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/handlers/runtime/PreProcessor.cpp"

	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/registries/BlockRegistry.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/registries/BlockRegistry.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/registries/char_texture.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/registries/char_texture.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/registries/ShaderRegistry.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/registries/ShaderRegistry.cpp"
	
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/Plane.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/Plane.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/Collection.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/Collection.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/Stack.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/Stack.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/Block.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/Block.cpp"
	
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/args/Argument.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/args/Argument.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/args/ArgumentAny.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/args/ArgumentAny.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/args/ArgumentBoolean.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/args/ArgumentBoolean.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/args/ArgumentDropdown.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/args/ArgumentDropdown.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/args/ArgumentReal.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/args/ArgumentReal.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/args/ArgumentString.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/args/ArgumentString.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/args/ArgumentText.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/stacking/args/ArgumentText.cpp"
	
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/ProgressBar.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/ProgressBar.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/Renderer.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/Renderer.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/RenderFrame.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/RenderFrame.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/RenderLayer.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/RenderLayer.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/RenderObject.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/RenderObject.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/RenderTools.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/RenderTools.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/RenderRectangle.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/RenderRectangle.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/RenderText.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/RenderText.cpp"
	# "${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/RenderUI.hpp"
	# "${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/RenderUI.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/Vertex.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/Vertex.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/Color.hpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/Color.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/RenderButton.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/RenderButton.cpp"

	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/interfaces/IPositionable.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/interfaces/IPositionable.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/interfaces/ISizeable.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/interfaces/ISizeable.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/interfaces/IScalable.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/interfaces/IScalable.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/interfaces/IRenderable.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/interfaces/IRenderable.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/interfaces/IUpdatable.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/interfaces/IUpdatable.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/interfaces/IUI.hpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/interfaces/IUI.cpp"
	# "${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/interfaces/IWeak.hpp"
	# "${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/interfaces/IWeak.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/interfaces/IColorable.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/interfaces/IColorable.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/interfaces/IDiColorable.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/interfaces/IDiColorable.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/interfaces/IEnableable.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/interfaces/IEnableable.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/interfaces/IBoundedParent.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rhr/ui/interfaces/IBoundedParent.cpp"
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