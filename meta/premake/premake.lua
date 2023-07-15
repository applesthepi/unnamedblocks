workspace "unnamedblocks"
	architecture "x86_64"
	configurations { "debug", "release" }
	flags {
		"MultiProcessorCompile"
	}
project "ub_latte"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	targetdir "../build/%{cfg.buildcfg}/ub_latte"
	objdir "../build/int/%{cfg.buildcfg}/ub_latte"
	location "../build"
	staticruntime "off"
	runtime "release"
	files {
		"../../ub_latte/**.hpp",
		"../../ub_latte/**.cpp"
	}
	includedirs {
		"../../ub_latte/include"
		-- "../dependencies/vp-engine/include"
	}
	libdirs {
		-- os.findlib("vulkan-1.lib")
	}
	links {
		-- "../dependencies/vp-engine/lib/%{cfg.buildcfg}/vpe.lib",
		-- windows libs for rust vpe static lib
		"wsock32",
		"ws2_32",
		"iphlpapi",
		"userenv",
		"psapi",
		"ntdll",
		"bcrypt",
		"imm32",
		"winmm",
		"uxtheme",
		"dwmapi",
		-- "vulkan-1.lib"
	}
	filter "configurations:debug"
		defines { "DEBUG" }
		symbols "On"
	filter "configurations:release"
		defines { "NDEBUG" }
		optimize "On"
project "ub_client"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	targetdir "../build/%{cfg.buildcfg}/ub_client"
	objdir "../build/int/%{cfg.buildcfg}/ub_client"
	location "../build"
	staticruntime "off"
	runtime "release"
	files {
		"../../ub_client/**.hpp",
		"../../ub_client/**.cpp"
	}
	includedirs {
		"../../ub_client/src",
		"../../ub_latte/include",
		"../dependencies/vp-engine/include"
	}
	libdirs {
		os.findlib("vulkan-1.lib"),
		"../dependencies/vp-engine/lib/%{cfg.buildcfg}"
	}
	links {
		"ub_latte",
		"vpe.lib",
		-- windows libs for rust vpe static lib
		"wsock32",
		"ws2_32",
		"iphlpapi",
		"userenv",
		"psapi",
		"ntdll",
		"bcrypt",
		"imm32",
		"winmm",
		"uxtheme",
		"dwmapi",
		"vulkan-1.lib"
	}
	filter "configurations:debug"
		defines { "DEBUG" }
		symbols "On"
	filter "configurations:release"
		defines { "NDEBUG" }
		optimize "On"
	filter { "system:windows" }
		postbuildcommands {
			"mkdir ..\\build\\res",
			"xcopy ..\\dependencies\\vp-engine\\res ..\\build\\res /sy",
			"xcopy ..\\resources ..\\build\\res /sy"
		}