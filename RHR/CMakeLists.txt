cmake_minimum_required(VERSION 3.10)

project(RHR)
configure_file(config.h.in config.h)
set(RHR_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/handlers/MessageHandler.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/handlers/ProjectHandler.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/handlers/runtime/PreProcessor.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/handlers/StatmentHandler.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/handlers/ContextHandler.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/registries/BlockRegistry.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/registries/UIRegistry.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/registries/ShaderRegistry.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/args/Argument.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/Plane.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/Collection.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/Stack.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/Block.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/StatmentIf.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/Button.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/ButtonText.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/ButtonImage.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/IMouseUpdatable.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/IRenderable.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/ITransformable.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/IUI.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/Dropdown.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/Field.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/handlers/CategoryHandler.cpp"
	#${CMAKE_CURRENT_SOURCE_DIR}/ Headers
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/handlers/MessageHandler.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/handlers/ProjectHandler.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/handlers/runtime/PreProcessor.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/handlers/StatmentHandler.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/handlers/ContextHandler.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/registries/BlockRegistry.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/registries/UIRegistry.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/registries/ShaderRegistry.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/RHR.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/args/Argument.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/args/ArgumentBoolean.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/args/ArgumentString.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/args/ArgumentText.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/args/ArgumentReal.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/args/ArgumentAny.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/args/ArgumentDropdown.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/Plane.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/Collection.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/Stack.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/stacking/Block.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/StatmentIf.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/Button.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/ButtonText.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/ButtonImage.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/IMouseUpdatable.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/IRenderable.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/IUI.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/ITransformable.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/Dropdown.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/ui/Field.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/handlers/CategoryHandler.h"
	
	#IMGUI-SFML
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/imgui/imconfig.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/imgui/imconfig-SFML.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/imgui/imgui.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/imgui/imgui_internal.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/imgui/imgui-SFML.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/imgui/imgui-SFML_export.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/imgui/imstb_rectpack.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/imgui/imstb_textedit.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/imgui/imstb_truetype.h"
	#IMGUI-SFML
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/imgui/imgui.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/imgui/imgui_demo.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/imgui/imgui_draw.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/imgui/imgui_widgets.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/RHR/imgui/imgui-SFML.cpp"
	)
add_library(RHR STATIC
	${RHR_SRC}
)
target_link_libraries(RHR PUBLIC
	Espresso
	Cappuccino
	sfml-graphics
	sfml-system
	sfml-window
	sfml-network
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
