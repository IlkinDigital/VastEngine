#include "vastpch.h"
#include "ProjectGenerator.h"

namespace Vast {

    void ProjectGenerator::GenerateDirectories()
    {
        std::filesystem::create_directories(m_Project->GetProjectPath() / "Content" / "Assets");
        std::filesystem::create_directories(m_Project->GetProjectPath() / "Vendor" / "premake");
        std::filesystem::create_directory(m_Project->GetProjectPath() / "Engine");
        std::filesystem::create_directory(m_Project->GetProjectPath() / "Source");
    }

    void ProjectGenerator::GeneratePremakeFile()
    {
        StringStream ss;
        ss
            << "workspace '" << m_Project->GetName() << "'"
            << R"(
    architecture 'x64'
    configurations{'Debug', 'Release', 'Distribution'}

outputdir = '%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}'
EnginePath = 'D:/Lester_Files/dev/Projects/VastEngine' 
)" // TODO: Remove hardcoded path to engine
<< "project '" << m_Project->GetName() << "'"
<< R"(
    kind 'SharedLib'
    language 'C++'
    cppdialect 'C++20'
    staticruntime 'off'

    targetdir('Binaries/' ..outputdir .. '/%{prj.name}')
    objdir('Binaries-Int/' ..outputdir .. '/%{prj.name}')

    files {'Source/**.h', 'Source/**.cpp'}

    pchheader "enginepch.h"
    pchsource "Source/enginepch.cpp"

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
        'Engine/Glad.lib',
        'Engine/yaml-cpp.lib'
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

        std::ofstream fs(m_Project->GetProjectPath() / "premake5.lua");
        fs << ss.str();
        fs.close();
    }

    void ProjectGenerator::DownloadDependencies(const Filepath& dest)
    {
        Filepath binPath = std::filesystem::current_path().parent_path() / "Binaries";

        // Define binaries folder based on build config and platform
        String binFolder;
#ifdef VAST_CONFIG_DEBUG
        binFolder += "Debug-";
#elif VAST_CONFIG_RELEASE
        binFolder += "Release-";
#elif VAST_CONFIG_DISTRIBUTION
        binFolder += "Distribution-";
#endif

#ifdef VAST_PLATFORM_WINDOWS
        binFolder += "windows-x86_64";
#endif

        binPath /= binFolder;

        if (!std::filesystem::exists(binPath))
        {
            VAST_CORE_ASSERT(false, "Invalid path to binaries");
            return;
        }

        std::filesystem::copy_file(binPath / "Vast/Vast.lib", m_Project->GetProjectPath() / dest / "Vast.lib");
        std::filesystem::copy_file(binPath / "Glad/Glad.lib", m_Project->GetProjectPath() / dest / "Glad.lib");
        std::filesystem::copy_file(binPath / "GLFW/GLFW.lib", m_Project->GetProjectPath() / dest / "GLFW.lib");
        std::filesystem::copy_file(binPath / "ImGui/ImGui.lib", m_Project->GetProjectPath() / dest / "ImGui.lib");
        std::filesystem::copy_file(binPath / "spdlog/spdlog.lib", m_Project->GetProjectPath() / dest / "spdlog.lib");
    
        Filepath pbSrc = std::filesystem::current_path().parent_path() / "Vendor/premake/premake5.exe";
        Filepath pbDest = dest.parent_path() / "Vendor/premake/premake5.exe";
        VAST_CORE_WARN("{0}", std::filesystem::copy_file(pbSrc, pbDest));
    }

}