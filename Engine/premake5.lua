include "./python-script/premake5.lua"

project "Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.hh",
		"src/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
		"vendor/ImGuiColorTextEdit/**.cpp",
		"vendor/ImGuiColorTextEdit/**.h",
		"vendor/stb_image/**.cpp",
		"vendor/stb_image/**.h",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE",
		"GLM_ENABLE_EXPERIMENTAL"
	}

	includedirs
	{
		"src",
		"vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.Portable_File_Dialogs}",
		"%{IncludeDir.ImGuiColorTextEdit}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.pybind11}",
		pythonIncludePath,
	}

	libdirs
	{
		pythonLibPath
	}

	filter "system:linux"
		pic "On"
		systemversion "latest"
		links
		{
			"X11",
			"dl",
			"python3.lib",
			"pthread",
			"GLFW",
			"Glad",
			"ImGui",
			"yaml-cpp",
			"GL",
		}
		defines
		{
			HZ_PLATFORM_LINUX
		}
	filter "system:windows"
		systemversion "latest"

		links
		{
			"python3.lib",
			"GLFW",
			"Glad",
			"ImGui",
			"yaml-cpp",
			"opengl32.lib"
		}

		defines
		{
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
