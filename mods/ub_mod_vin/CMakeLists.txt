cmake_minimum_required(VERSION 3.15)
project(UB_MOD_VIN)

set(UB_MOD_VIN_SRC

	#======================================================================================================================
	#=========== Mod
	#======================================================================================================================
	
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/mod.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/mod.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/categories.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/categories.hpp"
	
	#======================================================================================================================
	#=========== System
	#======================================================================================================================
	
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/main.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/main.cpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/pull_events.hpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/pull_events.cpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/CreateWindow.hpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/CreateWindow.cpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/WindowMouseX.hpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/WindowMouseX.cpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/WindowMouseY.hpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/WindowMouseY.cpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/WindowMouseLeft.hpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/WindowMouseLeft.cpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/WindowMouseMiddle.hpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/WindowMouseMiddle.cpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/WindowMouseRight.hpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/WindowMouseRight.cpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/WindowPullScroll.hpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/WindowPullScroll.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/sleep.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/sleep.cpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/WindowVsync.hpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/WindowVsync.cpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/WindowFps.hpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/WindowFps.cpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/WindowRender.hpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/system/WindowRender.cpp"
#	
#	#======================================================================================================================
#	#=========== Utility
#	#======================================================================================================================
#	
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/mark_point.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/mark_point.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/goto_point.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/goto_point.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/goto_point_if.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/goto_point_if.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/function_define.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/function_define.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/function_call.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/function_call.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/thread_summon.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/thread_summon.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/thread_kill.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/thread_kill.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/global_get_real.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/global_get_real.cpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/GlobalGetBool.hpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/GlobalGetBool.cpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/GlobalGetString.hpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/GlobalGetString.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/global_set_real.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/global_set_real.cpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/GlobalSetBool.hpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/GlobalSetBool.cpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/GlobalSetString.hpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/GlobalSetString.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/get_micro.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/get_micro.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/list_create.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/list_create.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/list_destroy.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/list_destroy.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/list_reserve.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/list_reserve.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/list_add.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/list_add.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/list_index_real.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/list_index_real.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/list_index_bool.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/list_index_bool.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/list_index_string.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/list_index_string.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/list_insert.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/list_insert.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/list_remove.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/list_remove.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/list_replace.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/list_replace.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/list_size.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/list_size.cpp"
#	
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/aid/global_real.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/aid/global_real.cpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/aid/UtilityGlobalBool.hpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/aid/UtilityGlobalBool.cpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/aid/UtilityGlobalString.hpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/aid/UtilityGlobalString.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/aid/list.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/utility/aid/list.cpp"
#	
#	#======================================================================================================================
#	#=========== Graphics
#	#======================================================================================================================
#	
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsTextureLoad.hpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsTextureLoad.cpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteCreate.hpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteCreate.cpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteDestroy.hpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteDestroy.cpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteTextureSet.hpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteTextureSet.cpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteEnable.hpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteEnable.cpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteDisable.hpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteDisable.cpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteSetPosition.hpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteSetPosition.cpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteSetRotation.hpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteSetRotation.cpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteSetScale.hpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteSetScale.cpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteChangePosition.hpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteChangePosition.cpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteChangeRotation.hpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteChangeRotation.cpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteChangeScale.cpp"
#	#"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/graphics/BlockGraphicsSpriteChangeScale.hpp"
#	
#	#======================================================================================================================
#	#=========== Booleans
#	#======================================================================================================================
#	
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/booleans/force.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/booleans/force.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/booleans/log.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/booleans/log.cpp"
#	
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/booleans/set.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/booleans/set.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/booleans/flip.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/booleans/flip.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/booleans/and.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/booleans/and.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/booleans/or.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/booleans/or.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/booleans/not.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/booleans/not.cpp"
#	
#	#======================================================================================================================
#	#=========== Reals
#	#======================================================================================================================
#	
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/force.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/force.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/log.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/log.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/set.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/set.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/plus.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/plus.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/minus.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/minus.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/divide.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/divide.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/multiply.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/multiply.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/mod.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/mod.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/floor.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/floor.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/ceil.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/ceil.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/round.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/round.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/from_bool.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/from_bool.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/from_string.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/from_string.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/pi.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/pi.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/eulers.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/eulers.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/random.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/random.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/abs.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/abs.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/pow.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/pow.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/logarithmic.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/logarithmic.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/sin.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/sin.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/cos.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/cos.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/tan.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/tan.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/comp_eq.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/comp_eq.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/comp_gr.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/comp_gr.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/comp_ls.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/comp_ls.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/comp_gr_eq.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/comp_gr_eq.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/comp_ls_eq.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/reals/comp_ls_eq.cpp"
#	
#	#======================================================================================================================
#	#=========== Strings
#	#======================================================================================================================
#	
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/force.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/force.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/log.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/log.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/sub_string.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/sub_string.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/char_at.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/char_at.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/comp_eq.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/comp_eq.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/length.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/length.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/set.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/set.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/from_real.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/from_real.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/from_real_int.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/from_real_int.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/from_bool.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/from_bool.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/find.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/find.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/cat_from.hpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin/blocks/strings/cat_from.cpp"
)

add_library(UB_MOD_VIN MODULE
	${UB_MOD_VIN_SRC}
)

if(MSVC)
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/mod_vin FILES ${UB_MOD_VIN_SRC})
	target_compile_options(UB_MOD_VIN PRIVATE "/MP")
endif()

add_dependencies(UB_MOD_VIN
    UB_CAPPUCCINO
    UB_ESPRESSO
    UB_RHR
)

set_target_properties(UB_MOD_VIN PROPERTIES OUTPUT_NAME "mod_vin")

target_link_libraries(UB_MOD_VIN PUBLIC
	UB_CAPPUCCINO
    UB_ESPRESSO
    UB_RHR
	
	glad::glad
	glfw
	imgui::imgui
	Vulkan::Vulkan
	freetype
)

target_include_directories(UB_MOD_VIN PUBLIC
	"${PROJECT_BINARY_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/ub_cappuccino/include"
    "${CMAKE_SOURCE_DIR}/ub_espresso/include"
    "${CMAKE_SOURCE_DIR}/ub_rhr/include"

    ${STB_INCLUDE_DIRS}
	${RAPIDJSON_INCLUDE_DIRS}
	${VULKAN_INCLUDE_DIRS}
)

add_custom_command(
	TARGET UB_MOD_VIN POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	$<TARGET_FILE:UB_MOD_VIN>
	$<TARGET_FILE_DIR:UB_CLIENT>/mods/
)