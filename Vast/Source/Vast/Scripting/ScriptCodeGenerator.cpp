#include "vastpch.h"
#include "ScriptCodeGenerator.h"

#include "Utils/FileIO/FileIO.h"

namespace Vast {

    static void IterateAndAddHeaders(const Filepath& start, DArray<Filepath>& arr)
    {
        for (auto& path : std::filesystem::directory_iterator(start))
        {
            if (path.is_directory() && path != start / "Generated")
                IterateAndAddHeaders(path, arr);
            else if (path.path().filename().extension() == ".h" && path.path().filename() != "enginepch.h")
                arr.emplace_back(path.path());
        }
    }

    void ScriptCodeGenerator::GeneratePCH()
    {
        Filepath root = m_Project->GetProjectPath() / "Source";

        String pchHeader = "#pragma once\n#include <Vast.h>\n";
        String pchCpp = "#include \"enginepch.h\"";

        std::ofstream fs(root / "enginepch.h");
        fs << pchHeader;
        fs.close();
        fs.open(root / "enginepch.cpp");
        fs << pchCpp;
        fs.close();
    }

	void ScriptCodeGenerator::GenerateExportFiles()
	{
        /**
        * User files preprocessing
        */

        DArray<Filepath> userFiles;
        DArray<String> userClasses;

        Filepath root = m_Project->GetProjectPath() / "Source";

        IterateAndAddHeaders(root, userFiles);

        // Search and add registered classes
        for (auto it = userFiles.begin(); it != userFiles.end();)
        {
            auto& file = *it;

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

            fs.close();

            if (!isScript)
                it = userFiles.erase(it);
            else
                it++;
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
R"(#include "enginepch.h"
#include "Generated.h"

)";

        // Add includes
        for (auto& file : userFiles)
        {
            String path = FileIO::Relative(file, m_Project->GetProjectPath() / "Source").string();
            if (path[0] == '\\')
                path.erase(path.begin());
            genCpp << "#include \"" << path << "\"\n";
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
        std::filesystem::create_directory(m_Project->GetProjectPath() / "Source" / "Generated");
        std::ofstream fsHeader(m_Project->GetProjectPath() / "Source" / "Generated" / "Generated.h");
        fsHeader << genHeader.str();
        fsHeader.close();
        std::ofstream fsCpp(m_Project->GetProjectPath() / "Source" / "Generated" / "Generated.cpp");
        fsCpp << genCpp.str();
        fsCpp.close();
	}

}