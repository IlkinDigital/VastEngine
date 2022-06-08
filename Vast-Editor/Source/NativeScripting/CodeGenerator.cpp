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
        ss
            << "workspace '" << m_Project.GetName() << "'"
            << R"(
    architecture 'x64'
    configurations{'Debug', 'Release', 'Distribution'}

outputdir = '%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}'
EnginePath = 'D:/Lester_Files/dev/Projects/VastEngine' 
)" // TODO: Remove hardcoded path to engine
<< "project '" << m_Project.GetName() << "'"
<< R"(
    kind 'SharedLib'
    language 'C++'
    cppdialect 'C++20'
    staticruntime 'on'

    targetdir('Binaries/' ..outputdir .. '/%{prj.name}')
    objdir('Binaries-Int/' ..outputdir .. '/%{prj.name}')

    files {'Source/**.h', 'Source/**.cpp'}

    pchheader "gamepch.h"
    pchsource "Source/gamepch.cpp"

    includedirs
    {
        'Source',
        '%{EnginePath}/Vast/Source',
        '%{EnginePath}/Vast/Source/Vast',
        '%{EnginePath}/Vast/Source/Vast/Core',
        '%{EnginePath}/Vast/Vendor/spdlog/include',
        '%{EnginePath}/Vast/Vendor/entt',
        '%{EnginePath}/Vast/Vendor/glm'
    }

    defines
    {
        '_CRT_SECURE_NO_WARNINGS',
        'VAST_SCRIPT_DLL'
    }

    links
    {
        'Engine/Vast.lib',
        'Engine/GLFW.lib',
        'Engine/ImGui.lib',
        'Engine/Glad.lib'
    }

    filter 'system:windows'
        systemversion 'latest'

    defines
    {
        'VAST_PLATFORM_WINDOWS'
    }

    filter 'configurations:Debug'
        defines 'VAST_CONFIG_DEBUG'
        runtime 'Debug'
        symbols 'on'

    filter 'configurations:Release'
        defines 'VAST_CONFIG_RELEASE'
        runtime 'Release'
        optimize 'on'

    filter 'configurations:Distribution'
        defines 'VAST_CONFIG_DISTRIBUTION'
        runtime 'Release'
        optimize 'on'
)";

        std::ofstream fs(m_Project.GetProjectPath() / "premake5.lua");
        fs << ss.str();
        fs.close();
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
            fs.close();
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
R"(#include "gamepch.h"
#include "Generated.h"

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
        fsHeader.close();
        std::ofstream fsCpp(m_Project.GetProjectPath() / "Source" / "Generated" / "Generated.cpp");
        fsCpp << genCpp.str();
        fsCpp.close();
	}

}