workspace "VastEngine"
    architecture "x64"
    --startproject "Vast-Editor"

    configurations
    {
        "Debug", -- Debug features, logging
        "Release", -- Optimized debug config without debug features
        "Distribution" -- Final distributed configuration
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

Include = {}

project "Vast"
    location "Vast"
    language "C++"
    cppdialect "C++20"
    kind "StaticLib"
    staticruntime "on"

    targetdir ("Binaries/" .. outputdir .. "/%{prj.name}")
    objdir ("Binaries-Int" .. outputdir .. "/%{prj.name}")

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
        "%{prj.name}/Source"
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