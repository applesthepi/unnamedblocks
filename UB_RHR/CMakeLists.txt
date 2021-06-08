cmake_minimum_required(VERSION 3.15)
project(UB_RHR)

set(UB_RHR_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/handlers/MessageHandler.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/handlers/MessageHandler.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/handlers/ProjectHandler.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/handlers/ProjectHandler.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/handlers/StatmentHandler.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/handlers/StatmentHandler.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/handlers/ContextHandler.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/handlers/ContextHandler.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/handlers/CategoryHandler.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/handlers/CategoryHandler.cpp"

	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/handlers/runtime/PreProcessor.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/handlers/runtime/PreProcessor.cpp"

	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/registries/BlockRegistry.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/registries/BlockRegistry.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/registries/UIRegistry.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/registries/UIRegistry.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/registries/ShaderRegistry.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/registries/ShaderRegistry.cpp"
	
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/Plane.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/Plane.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/Collection.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/Collection.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/Stack.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/Stack.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/Block.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/Block.cpp"
	
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/args/Argument.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/args/Argument.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/args/ArgumentAny.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/args/ArgumentBoolean.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/args/ArgumentDropdown.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/args/ArgumentReal.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/args/ArgumentString.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/args/ArgumentText.hpp"
	
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/ProgressBar.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/ProgressBar.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/Renderer.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/Renderer.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/RenderFrame.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/RenderFrame.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/RenderLayer.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/RenderLayer.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/RenderObject.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/RenderObject.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/RenderRectangle.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/RenderRectangle.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/RenderTools.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/RenderTools.cpp"
	# "${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/RenderUI.hpp"
	# "${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/RenderUI.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/Vertex.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/Vertex.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/Color.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/Color.cpp"

	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/interfaces/IPositionable.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/interfaces/IPositionable.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/interfaces/ISizeable.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/interfaces/ISizeable.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/interfaces/IScalable.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/interfaces/IScalable.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/interfaces/IRenderable.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/interfaces/IRenderable.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/interfaces/IUpdatable.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/interfaces/IUpdatable.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/interfaces/IUI.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/interfaces/IUI.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/interfaces/IWeak.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/interfaces/IWeak.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/interfaces/IColorable.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/interfaces/IColorable.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/interfaces/IDiColorable.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/interfaces/IDiColorable.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/interfaces/IEnableable.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/interfaces/IEnableable.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/interfaces/IBoundedParent.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/interfaces/IBoundedParent.cpp"
)

# Preserve folders in visual studio structure
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/RHR FILES ${UB_RHR_SRC})

add_library(UB_RHR STATIC
	${UB_RHR_SRC}
)

target_link_libraries(UB_RHR PUBLIC
	UB_ESPRESSO
	UB_CAPPUCCINO
	"${CMAKE_SOURCE_DIR}/tcc/lib/libtcc1-64.a"
	glad::glad
	glm
	glfw
	imgui::imgui
	Vulkan::Vulkan
)

target_include_directories(UB_RHR PUBLIC
	"${PROJECT_BINARY_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR"
	"${CMAKE_SOURCE_DIR}/UB_Espresso/include"
	"${CMAKE_SOURCE_DIR}/UB_Cappuccino/include"
	"${CMAKE_SOURCE_DIR}/tcc/libtcc"
)

configure_file(config.h.in config.h)