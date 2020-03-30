workspace "ProjectIris"

    architecture "x64"
    startproject "Sandbox"

    configurations 
    {
        "Debug",
        "Release",
        "Dist"
    }

    flags
	{
		"MultiProcessorCompile"
    }
    
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "NURU/vendor/GLFW/include"
IncludeDir["Glad"] = "NURU/vendor/Glad/include"
IncludeDir["Stb"] = "NURU/vendor/stb"
IncludeDir["Glm"] = "NURU/vendor/glm"
IncludeDir["Assimp"] = "NURU/vendor/Assimp/include"
IncludeDir["AssimpBuild"] = "NURU/vendor/Assimp/build/include"
IncludeDir["ImGui"] = "NURU/vendor/ImGui"

include "NURU/vendor/GLFW"
include "NURU/vendor/Glad"
include "NURU/vendor/Assimp"
include "NURU/vendor/ImGui"

project "NURU"

    location "NURU"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin/int/" .. outputdir .. "/%{prj.name}")

    pchheader "NURUPCH.h"
    pchsource "NURU/src/NURUPCH.cpp"

    files 
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/stb/**.h",
        "%{prj.name}/vendor/stb/**.cpp",
    }

    defines 
    {
        "_CRT_SECURE_NO_WARNINGS",
    }

    includedirs 
    {
        "%{prj.name}/src/",
        "%{prj.name}/src/NURU/Core",
        "%{prj.name}/src/NURU/Renderer",
        "%{prj.name}/vendor/spdlog/include/",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Stb}",
        "%{IncludeDir.Assimp}",
        "%{IncludeDir.AssimpBuild}",
        "%{IncludeDir.ImGui}"
    }

    links 
    {
        "Assimp",
        "GLFW",
        "Glad",
        "opengl32.lib",
        "ImGui"
    }

    filter "system:windows"
        systemversion "latest"

        defines 
        {
            "NURU_PLATFORM_WINDOWS",
            "_WIN32"
        }


    filter "configurations:Debug"
        defines "NURU_DEBUG"
        symbols "on"
        runtime "Debug"

    filter "configurations:Release"
        defines "NURU_RELEASE"
        optimize "on"
        runtime "Release"

    filter "configurations:Dist"
        defines "NURU_DIST"
        optimize "on"
        runtime "Release"

project "Sandbox"
    location "Sandbox"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin/int/" .. outputdir .. "/%{prj.name}")

    files 
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "resources.rc", 
        "**.ico"
    }

    includedirs {
        "NURU/src",
        "NURU/vendor/spdlog/include/",
        "%{IncludeDir.Glad}", 
	    "%{IncludeDir.GLFW}",
        "%{IncludeDir.Stb}",
        "%{IncludeDir.Assimp}",
        "%{IncludeDir.AssimpBuild}",
        "%{IncludeDir.ImGui}"
    }

    links 
    {
        "NURU"
    }

    filter "system:windows"
        systemversion "latest"

        defines 
        {
            "_CRT_SECURE_NO_WARNINGS",
            "NURU_PLATFORM_WINDOWS",
        }

    filter "configurations:Debug"
        kind "ConsoleApp"
        defines "NURU_DEBUG"
        symbols "on"
        runtime "Debug"

    filter "configurations:Release"
        kind "WindowedApp"
        defines "NURU_RELEASE"
        optimize "on"
        runtime "Release"

    filter "configurations:Dist"
        kind "WindowedApp"
        defines "NURU_DIST"
        optimize "on"
        runtime "Release"