workspace "Vaulteer"
	architecture "x86_64"
	startproject "Vaulteer"

	configurations {
		"Debug",
		"Release"
	}

	buildoptions {
		"/MP32 /bigobj"
	}

	--flags { "MultiProcessorCompile" }


builddir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


-- include "Vaulteer"

project "Vaulteer"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++latest"

	targetdir ("%{wks.location}/bin/" .. builddir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-obj/" .. builddir .. "/%{prj.name}")

	pchheader "vpch.h"
	pchsource "src/vpch.cpp"

	files {
		"src/**",
		"resources/**",
		"vendor/GEM/GEM.h",
		
		"vendor/GLAD/src/**.c",
	}

	removefiles { 
		"resources/shaders/build/**"
	}

	includedirs {
		"%{wks.location}/src",
		"%{wks.location}/vendor/_headers",
		"%{wks.location}/vendor/ASSIMP/include",
		"%{wks.location}/vendor/GEM/include",
		"%{wks.location}/vendor/GLAD/include",
		"%{wks.location}/vendor/GLFW/include",
		"%{wks.location}/vendor/GLM/include",
		"%{wks.location}/vendor/miniball/include",
		"%{wks.location}/vendor/rectpack2D/include",
		"%{wks.location}/vendor/sqlite/include",
		"%{wks.location}/vendor/tomlplusplus-2.4.0/include",
	}

	libdirs {
		"%{wks.location}/vendor/ASSIMP/lib/%{cfg.buildcfg}",
		"%{wks.location}/vendor/GEM/lib/%{cfg.buildcfg}",
		"%{wks.location}/vendor/GLFW/lib/",
		"%{wks.location}/vendor/sqlite/lib/",
	}

	links {
		"glfw3",
		"opengl32"
	}

	vpaths {
	   ["Source Files/*"] = { "src/**.h", "src/**.cpp" },
	   ["Resource Files/*"] = { "resources/**", "**.dll", "**.toml" },
	   ["Vendor Files/*"] = { "vendor/**.h", "vendor/**.c" },
	}
	vpaths {
	   ["Resource Files/unspecified/*"] = { "resources/**", "**" },
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

		links {
			"assimp-vc142-mtd"
		}

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		inlining "Auto"

		links {
			"assimp-vc142-mt"
		}

	filter "files:**.c"
		flags { "NoPCH" }
