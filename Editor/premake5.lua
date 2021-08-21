include "./python-script/premake5.lua"

project "Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.hh",
		"src/**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLM_ENABLE_EXPERIMENTAL",
	}

	includedirs
	{
		"%{wks.location}/Engine/vendor/spdlog/include",
		"%{wks.location}/Engine/src",
		"%{wks.location}/Engine/vendor",
		"%{wks.location}/Engine/vendor/pybind11",
		pythonIncludePath,
		"%{IncludeDir.glm}",
	}
	libdirs
	{
		pythonLibPath
	}
	filter "system:linux"
		links
		{
			"Engine",
			"python3.lib",
			"dl",
			"pthread",
			"X11",
			"GL",
			"Glad",
			"GLFW",
			"ImGui",
			"freetype",
			"yaml-cpp",
		}

	filter "system:windows"
		systemversion "latest"
		links
		{
			"Engine",
			"python3.lib",
			"GLFW",
			"Glad",
			"ImGui",
			"freetype",
			"yaml-cpp"
		}

	filter "configurations:Debug"
		defines "RYM_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RYM_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "RYM_DIST"
		runtime "Release"
		optimize "on"
