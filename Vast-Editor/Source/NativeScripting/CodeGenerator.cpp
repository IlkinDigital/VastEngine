#include "CodeGenerator.h"

#include <fstream>

namespace Vast {

	void CodeGenerator::GenerateProjectFile()
	{
        StringStream ss;
        const char* tab = "    ";
        ss << "workspace '" << m_Project.GetName() << "'\n"
            << tab << "architecture 'x64'\n"
            << tab << "configurations{'Debug', 'Release', 'Distribution'}\n\n"
            << "outputdir = '%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}'\n"
            << "EnginePath = 'D:/Lester_Files/dev/Projects/VastEngine'\n\n" // TODO: Remove hardcoded path to engine
            << "project '" << m_Project.GetName() << "'\n"
            << tab << "kind 'SharedLib'\n"
            << tab << "language 'C++'\n"
            << tab << "cppdialect 'C++20'\n"
            << tab << "staticruntime 'off'\n\n"
            << tab << "targetdir('Binaries/' ..outputdir .. '/%{prj.name}')\n"
            << tab << "objdir('Binaries-Int/' ..outputdir .. '/%{prj.name}')\n\n"
            << tab << "files {'Source/**.h', 'Source/**.cpp'}\n\n"

            << tab << "includedirs\n"
            << tab << "{\n"
            << tab << tab << "'%{EnginePath}/Vast/Source',\n"
            << tab << tab << "'%{EnginePath}/Vast/Source/Vast',\n"
            << tab << tab << "'%{EnginePath}/Vast/Source/Vast/Core',\n"
            << tab << tab << "'%{EnginePath}/Vast/Vendor/spdlog/include',\n"
            << tab << tab << "'%{EnginePath}/Vast/Vendor/entt',\n"
            << tab << tab << "'%{EnginePath}/Vast/Vendor/glm'\n"
            << tab << "}\n\n"

            << tab << "defines\n"
            << tab << "{\n"
            << tab << tab << "'_CRT_SECURE_NO_WARNINGS',\n"
            << tab << tab << "'VAST_SCRIPT_DLL'\n"
            << tab << "}\n\n"

            << tab << "links\n"
            << tab << "{\n"
            << tab << tab << "'Engine/Vast.lib',\n"
            << tab << tab << "'Engine/GLFW.lib',\n"
            << tab << tab << "'Engine/ImGui.lib',\n"
            << tab << tab << "'Engine/Glad.lib'\n"
            << tab << "}\n\n"

            << tab << "filter{ 'system:windows', 'configurations:Debug' }\n"
            << tab << tab << "buildoptions '/MDd'\n"

            << tab << "filter{ 'system:windows', 'configurations:Release' }\n"
            << tab << tab << "buildoptions '/MD'\n"

            << tab << "filter 'system:windows'\n"
            << tab << tab << "systemversion 'latest'\n"

            << tab << "defines\n"
            << tab << "{\n"
            << tab << tab << "'VAST_PLATFORM_WINDOWS'\n"
            << tab << "}\n"

            << tab << "filter 'configurations:Debug'\n"
            << tab << tab << "defines 'VAST_CONFIG_DEBUG'\n"
            << tab << tab << "runtime 'Debug'\n"
            << tab << tab << "symbols 'on'\n"

            << tab << "filter 'configurations:Release'\n"
            << tab << tab << "defines 'VAST_CONFIG_RELEASE'\n"
            << tab << tab << "runtime 'Release'\n"
            << tab << tab << "optimize 'on'\n"

            << tab << "filter 'configurations:Distribution'\n"
            << tab << tab << "defines 'VAST_CONFIG_DISTRIBUTION'\n"
            << tab << tab << "runtime 'Release'\n"
            << tab << tab << "optimize 'on'\n";

        std::ofstream fs(m_Project.GetProjectPath() / "premake5.lua");
        fs << ss.str();
	}

	void CodeGenerator::GenerateExportFile()
	{
	}

}