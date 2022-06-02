workspace "Vaulteer"
	architecture "x86_64"
	startproject "Vaulteer"

	configurations {
		"Debug",
		"Release"
	}

	buildoptions {
		"/MP32"
	}

	--flags {
	--	"MultiProcessorCompile"
	--}


builddir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


-- include "Vaulteer"

project "Vaulteer"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++latest"

	targetdir ("%{wks.location}/bin/" .. builddir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-obj/" .. builddir .. "/%{prj.name}")

	files {
		"src/**",
		"resources/**",
		--"%{wks.location}/GLSLCPPBinder.h",
		"%{wks.location}/vendor/GLAD/src/**.c",
	}

	includedirs {
		"%{wks.location}/vendor/ASSIMP/include",
		"%{wks.location}/vendor/DebugYourself/include",
		"%{wks.location}/vendor/GLAD/include",
		"%{wks.location}/vendor/GLFW/include",
		"%{wks.location}/vendor/GLM/include",
		"%{wks.location}/vendor/miniball/include",
		"%{wks.location}/vendor/rectpack2D/include",
		"%{wks.location}/vendor/sqlite/include",
		"%{wks.location}/vendor/stb_image"
	}

	libdirs {
		"%{wks.location}/vendor/ASSIMP/lib/%{cfg.buildcfg}",
		"%{wks.location}/vendor/DebugYourself/lib/%{cfg.buildcfg}",
		"%{wks.location}/vendor/sqlite/lib/",
		"%{wks.location}/vendor/GLFW/lib/",
	}

	links {
		"glfw3",
		"opengl32"
	}

	vpaths {
	   ["Source Files/*"] = { "src/**.h", "src/**.cpp" },
	   ["Resource Files/*"] = { "resources/**", "**.dll", "**.toml" },
	   ["Vendor Files/*"] = { "*.h", "vendor/**.c" },
	}
	vpaths {
	   ["Resource Files/unspecified/*"] = { "resources/**", "**" },
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

		links {
			"assimp-vc142-mt"
		}

	filter "files:**.c"
		flags { "NoPCH" }
