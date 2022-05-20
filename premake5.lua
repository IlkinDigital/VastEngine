workspace "VastEngine"
    architecture "x64"
    startproject "Vast-Editor"

    configurations
    {
        "Debug", -- Debug features, logging
        "Release", -- Optimized debug config without debug features
        "Distribution" -- Final distributed configuration
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
    include "Vast/Vendor/spdlog"
    include "Vast/Vendor/glfw"
    include "Vast/Vendor/glad"
    include "Vast/Vendor/imgui"
group ""

Include = {}
Include["spdlog"] = "Vast/Vendor/spdlog/include"
Include["glfw"] = "Vast/Vendor/glfw/include"
Include["glad"] = "Vast/Vendor/glad/include"
Include["imgui"] = "Vast/Vendor/imgui"
Include["glm"] = "Vast/Vendor/glm"
Include["stb"] = "Vast/Vendor/stb"
Include["entt"] = "Vast/Vendor/entt"

project "Vast"
    location "Vast"
    language "C++"
    cppdialect "C++20"
    kind "StaticLib"
    staticruntime "on"

    targetdir ("Binaries/" .. outputdir .. "/%{prj.name}")
    objdir ("Binaries-Int/" .. outputdir .. "/%{prj.name}")

    pchheader "vastpch.h"
    pchsource "Vast/Source/vastpch.cpp"

    files
    {
        "%{prj.name}/Source/**.h",
        "%{prj.name}/Source/**.cpp",
        "%{prj.name}/Vendor/glm/glm/**.inl",
        "%{prj.name}/Vendor/glm/glm/**.hpp",       
        "%{prj.name}/Vendor/stb/**.h",
        "%{prj.name}/Vendor/stb/**.cpp",
        "%{prj.name}/Vendor/entt/**.hpp"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs
    {
        "%{prj.name}/Source",
        "%{prj.name}/Source/Vast",
        "%{prj.name}/Source/Vast/Core", -- to easily access commonly used headers
        "%{Include.spdlog}",
        "%{Include.glfw}",
        "%{Include.glad}",
        "%{Include.imgui}",
        "%{Include.glm}",
        "%{Include.stb}",
        "%{Include.entt}"
    }

    links
    {
        "spdlog",
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "VAST_PLATFORM_WINDOWS",
            "GLFW_INCLUDE_NONE"
        }

    filter "configurations:Debug"
        defines "VAST_CONFIG_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "VAST_CONFIG_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Distribution"
        defines "VAST_CONFIG_DISTRIBUTION"
        runtime "Release"
        optimize "on"

project "Vast-Editor"
    location "Vast-Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("Binaries/" .. outputdir .. "/%{prj.name}")
    objdir ("Binaries-Int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/Source/**.h",
        "%{prj.name}/Source/**.cpp"
    }

    includedirs
    {
        "Vast/Source",
        "Vast/Source/Vast",
        "Vast/Source/Vast/Core",
        "%{Include.spdlog}",
        "%{Include.glad}",
        "%{Include.imgui}",
        "%{Include.glm}",
        "%{Include.entt}"
    }

    links
    {
        "Vast"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "VAST_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "VAST_CONFIG_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "VAST_CONFIG_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Distribution"
        defines "VAST_CONFIG_DISTRIBUTION"
        runtime "Release"
        optimize "on"