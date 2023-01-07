workspace "LearnOpenGL"
	architecture "x64"
	startproject "Sandbox"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "vendor/GLFW/include"
IncludeDir["Glad"] = "vendor/Glad/include"
IncludeDir["glm"] = "vendor/glm/glm"

include "vendor/Glad"
include "vendor/GLFW"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir    ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	defines {
		"GLEW_STATIC",
		"_WIN32"
	}

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/vendor",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.glm}"
	}

	links { 
		"GLFW",
		"GLad",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "LO_DEBUG"
		runtime "Debug"
		symbols "on"
		
	filter "configurations:Release"
		defines "LO_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "LO_DIST"
		runtime "Release"
		optimize "on"

		
