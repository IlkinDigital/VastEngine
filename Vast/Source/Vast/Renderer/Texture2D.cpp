#include "vastpch.h"
#include "Texture2D.h"

#include "Platform/OpenGL/OpenGLTexture2D.h"

namespace Vast {

    Ref<Texture2D> Texture2D::Create(uint32 width, uint32 height)
    {
        return CreateRef<OpenGLTexture2D>(width, height);
    }

    Ref<Texture2D> Vast::Texture2D::Create(const String& filepath)
    {
        return CreateRef<OpenGLTexture2D>(filepath);
    }

}