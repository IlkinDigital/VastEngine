#include "vastpch.h"
#include "GLSLPreprocessor.h"

namespace Vast {

    GLSLPreprocessor::Shaders GLSLPreprocessor::PreprocessSource(const String& source)
    {
        // Get location of shader types

        auto vertexStart = source.find("#vertex");
        if (vertexStart == String::npos)
        {
            VAST_CORE_ASSERT(false, "Vertex shader(#vertex) wasn't defined");
            return Shaders();
        }

        auto fragStart = source.find("#fragment");
        if (fragStart == String::npos)
        {
            VAST_CORE_ASSERT(false, "Fragment shader(#fragment) wasn't defined");
            return Shaders();
        }

        Shaders shaders;

        shaders.VertexShaderSource = source.substr(vertexStart + 7, fragStart - 7);
        shaders.FragmentShaderSource = source.substr(fragStart + 9, source.size() - 1);

        return shaders;
    }

    GLSLPreprocessor::Shaders GLSLPreprocessor::PreprocessFile(const String& filepath)
    {
        std::ifstream fs(filepath);

        // Is file opened check
        if (!fs.is_open())
        {
            VAST_CORE_ASSERT(fs.is_open(), "File couldn't be opened");
            return Shaders();
        }

        String fullSource;
        String line;
        while (std::getline(fs, line))
        {
            fullSource += line + '\n';
        }

        return PreprocessSource(fullSource);
    }

}