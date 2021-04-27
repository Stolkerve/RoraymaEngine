include "./vendor/premake/premake_customization/solution_items.lua"

workspace "RoraymaEngine"
	architecture "x86_64"
	startproject "RoraymaEditor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Engine/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Engine/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Engine/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/Engine/vendor/glm"
IncludeDir["Portable_File_Dialogs"] = "%{wks.location}/Engine/vendor/Portable_File_Dialogs"
IncludeDir["ImGuiColorTextEdit"] = "%{wks.location}/Engine/vendor/ImGuiColorTextEdit"
IncludeDir["stb_image"] = "%{wks.location}/Engine/vendor/stb_image"
IncludeDir["yaml_cpp"] = "%{wks.location}/Engine/vendor/yaml-cpp/include"
IncludeDir["pybind11"] = "%{wks.location}/Engine/vendor/pybind11"

group "Dependencies"
	include "vendor/premake"
	include "Engine/vendor/GLFW"
	include "Engine/vendor/Glad"
	include "Engine/vendor/imgui"
	include "Engine/vendor/yaml-cpp"
group ""

include "Engine"
include "Editor"