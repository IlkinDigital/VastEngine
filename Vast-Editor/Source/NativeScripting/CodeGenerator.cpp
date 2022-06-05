#include "CodeGenerator.h"

#include <fstream>

namespace Vast {

    static void IterateAndAddHeaders(const Filepath& start, DArray<Filepath>& array)
    {
        for (auto& path : std::filesystem::directory_iterator(start))
        {
            if (path.is_directory() && path != start / "Generated")
                IterateAndAddHeaders(path, array);
            else if (path.path().filename().extension() == ".h")
                array.emplace_back(path.path());
        }
    }

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
            << tab << tab << "'Source',\n"
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
            << tab << tab << "buildoptions '/MTd'\n"

            << tab << "filter{ 'system:windows', 'configurations:Release' }\n"
            << tab << tab << "buildoptions '/MT'\n"

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
        /**
        * User files preprocessing
        */

        DArray<Filepath> userFiles;
        DArray<String> userClasses;

        Filepath root = m_Project.GetProjectPath() / "Source";

        IterateAndAddHeaders(root, userFiles);

        // Search and add registered classes
        int i = 0;
        for (auto& file : userFiles)
        {
            std::ifstream fs(file);
            
            bool isScript = false;
            String line;
            while (std::getline(fs, line))
            {
                auto index = line.find("REG_CLASS");
                if (index != String::npos)
                {
                    String className;
                    for (uint16 i = index + 10; i < line.size(); i++)
                    {
                        if (line[i] == ')')
                            break;
                        className += line[i];
                    }

                    if (!className.empty())
                    {
                        userClasses.emplace_back(className);
                        isScript = true;
                    }
                }
            }

            if (!isScript)
                userFiles.erase(userFiles.begin() + i);

            i++;
        }

        /**
        * Generating Header file
        */

        StringStream genHeader;

        genHeader << 
R"(
#include "Vast/Scene/Components/NativeScriptComponent.h"
#include "Vast/Scene/Entity.h"
#include "Vast/ApplicationCore/Application.h"

#define SCRIPT_API __declspec(dllexport)

extern "C"
{
	using ScriptBuffer = Vast::DArray<Vast::NativeScriptComponent>;

	SCRIPT_API const ScriptBuffer& GetScripts();
	SCRIPT_API void InitScripts();
	SCRIPT_API void InitModule(Vast::Application* appPtr);
}
)";

        StringStream genCpp;

        genCpp << 
R"(
#include "Generated.h"

#include "Vast/ApplicationCore/Application.h"
#include <unordered_map>

)";

        // Add includes
        for (auto& file : userFiles)
        {
            genCpp << "#include \"" << file.filename().string() << "\"\n";
        }

        genCpp << 
R"(
extern "C"
{
	static ScriptBuffer s_Scripts;

	SCRIPT_API const ScriptBuffer& GetScripts()
	{
		return s_Scripts;
	}

	SCRIPT_API void InitScripts()
	{
)";

        // Binding and adding scripts to buffer
        int id = 0;
        for (auto& className : userClasses)
        {
            genCpp
                << "\t\tVast::NativeScriptComponent var" << id << ";\n"
                << "\t\tvar" << id << ".Bind<" << className << ">();\n"
                << "\t\tvar" << id << ".Name = \"" << className << "\";\n"
                << "\t\ts_Scripts.emplace_back(var" << id << ");\n";
            id++;
        }

        genCpp << 
R"(
	}

	SCRIPT_API void InitModule(Vast::Application* appPtr)
	{
		Vast::Application::Set(appPtr);
		Vast::Logger::Init();
		Vast::Application::Get().GetWindow().Init();
	}
}
)";

        /**
        * Writing generated files
        */
        std::filesystem::create_directory(m_Project.GetProjectPath() / "Source" / "Generated");
        std::ofstream fsHeader(m_Project.GetProjectPath() / "Source" / "Generated" / "Generated.h");
        fsHeader << genHeader.str();
        std::ofstream fsCpp(m_Project.GetProjectPath() / "Source" / "Generated" / "Generated.cpp");
        fsCpp << genCpp.str();
	}

}