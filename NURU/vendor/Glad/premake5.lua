project "Glad"
    kind "StaticLib"
	language "C"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/int/" .. outputdir .. "/%{prj.name}")

	includedirs {
		"include/"
	}

	files
	{
		"glad.c"
	}