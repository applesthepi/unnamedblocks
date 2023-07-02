workspace "unnamedblocks"
	configurations { "Debug", "Release" }

project "ub_client"
	kind "ConsoleApp"
	language "C++"
	targetdir "../build/%{cfg.buildcfg}"
	location "../build"
	
	files { "../../ub_client/src/**.hpp", "../../ub_client/src/**.cpp" }
	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"