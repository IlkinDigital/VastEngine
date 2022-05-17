#include "vastpch.h"
#include "GLSLPreprocessor.h"

namespace Vast {

    GLSLPreprocessor::Shaders GLSLPreprocessor::Preprocess(const String& GLSLSource)
    {
        uint32 vertexStart = GLSLSource.find("#vertex");
        VAST_CORE_ASSERT(vertexStart != String::npos, "Vertex shader(#vertex) wasn't defined");

        uint32 fragStart = GLSLSource.find("#fragment");
        VAST_CORE_ASSERT(fragStart != String::npos, "Fragment shader(#fragment) wasn't defined");

        Shaders shaders;
        return shaders;
    }

}