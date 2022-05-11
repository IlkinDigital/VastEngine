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

Include = {}
Include["spdlog"] = "Vast/Vendor/spdlog/include"
Include["glfw"] = "Vast/Vendor/glfw/include"

group "Dependencies"
    include "Vast/Vendor/spdlog"
    include "Vast/Vendor/glfw"
group ""

project "Vast"
    location "Vast"
    language "C++"
    cppdialect "C++20"
    kind "StaticLib"
    staticruntime "on"

    targetdir ("Binaries/" .. outputdir .. "/%{prj.name}")
    objdir ("Binaries-Int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/Source/**.h",
        "%{prj.name}/Source/**.cpp"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs
    {
        "%{prj.name}/Source",
        "%{prj.name}/Source/Vast/Core", -- to easily access commonly used headers
        "%{Include.spdlog}",
        "%{Include.glfw}"
    }

    links
    {
        "spdlog",
        "glfw"
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
        "Vast/Source/Vast/Core",
        "%{Include.spdlog}"
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