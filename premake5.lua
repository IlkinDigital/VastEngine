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
    include "Vast/Vendor/yaml-cpp"
group ""

Include = {}
Include["spdlog"] = "Vast/Vendor/spdlog/include"
Include["glfw"] = "Vast/Vendor/glfw/include"
Include["glad"] = "Vast/Vendor/glad/include"
Include["imgui"] = "Vast/Vendor/imgui"
Include["glm"] = "Vast/Vendor/glm"
Include["stb"] = "Vast/Vendor/stb"
Include["entt"] = "Vast/Vendor/entt"
Include["ImGuizmo"] = "Vast/Vendor/ImGuizmo"
Include["yaml_cpp"] = "Vast/Vendor/yaml-cpp/include"
Include["Optick"] = "Vast/Vendor/Optick/include"

project "Vast"
    location "Vast"
    language "C++"
    cppdialect "C++20"
    kind "StaticLib"
    staticruntime "off"

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
        "%{prj.name}/Vendor/entt/**.hpp",
        "%{prj.name}/Vendor/ImGuizmo/ImGuizmo.h",
        "%{prj.name}/Vendor/ImGuizmo/ImGuizmo.cpp"
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
        "Vast/Vendor/",
        "%{Include.glad}",
        "%{Include.imgui}",
        "%{Include.glm}",
        "%{Include.stb}",
        "%{Include.entt}",
        "%{Include.ImGuizmo}",
        "%{Include.yaml_cpp}",
        "%{Include.Optick}"
    }

    links
    {
        "spdlog",
        "GLFW",
        "Glad",
        "ImGui",
        "yaml-cpp",
        "opengl32.lib",
        "Vast/Vendor/Optick/lib/x64/release/OptickCore.lib"
    }

    filter "files:Vast/Vendor/ImGuizmo/**.cpp"
        flags { "NoPCH" }

    filter "files:Vast/Vendor/Optick/src/*.cpp"
        flags { "NoPCH" }

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

        -- links
        -- {
        --     "Vast/Vendor/Optick/lib/x64/debug/OptickCore.lib"
        -- }

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
    staticruntime "off"

    targetdir ("Binaries/" .. outputdir .. "/%{prj.name}")
    objdir ("Binaries-Int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/Source/**.h",
        "%{prj.name}/Source/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/Source",
        "Vast/Source",
        "Vast/Source/Vast",
        "Vast/Source/Vast/Core",
        "%{Include.spdlog}",
        "%{Include.glad}",
        "%{Include.imgui}",
        "%{Include.glm}",
        "%{Include.entt}",
        "%{Include.ImGuizmo}",
        "%{Include.Optick}"
    }

    links
    {
        "Vast",
        "Vast/Vendor/Optick/lib/x64/release/OptickCore.lib"
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
        