#include "vastpch.h"
#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Vast {

    Ref<Shader> Shader::Create(const String& name, const String& vertSource, const String& fragSource)
    {
        return CreateRef<OpenGLShader>(name, vertSource, fragSource);
    }

}