workspace "unnamedblocks"
	architecture "x86_64"
	configurations { "debug", "release" }
	flags {
		"MultiProcessorCompile"
	}

project "ub_client"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	targetdir "../build/%{cfg.buildcfg}"
	objdir "../build/int/%{cfg.buildcfg}"
	location "../build"
	staticruntime "off"
	runtime "release"
	
	files {
		"../../ub_client/src/**.hpp",
		"../../ub_client/src/**.cpp"
	}
	includedirs {
		"../dependencies/vp-engine/include"
	}
	links {
		"../dependencies/vp-engine/lib/%{cfg.buildcfg}/vpe.lib",
		-- windows libs for rust vpe static lib
		"wsock32",
		"ws2_32",
		"iphlpapi",
		"userenv",
		"psapi",
		"ntdll",
		"bcrypt"
	}
	filter "configurations:debug"
		defines {
			"DEBUG"
		}
		symbols "On"
	filter "configurations:release"
		defines { "NDEBUG" }
		optimize "On"