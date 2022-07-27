#include "vastpch.h"
#include "Texture2D.h"

#include "Platform/OpenGL/OpenGLTexture2D.h"

namespace Vast {

    Ref<Texture2D> Texture2D::Create(uint32 width, uint32 height)
    {
        return CreateRef<OpenGLTexture2D>(width, height);
    }

    Ref<Texture2D> Texture2D::Create(const Filepath& filepath)
    {
        return CreateRef<OpenGLTexture2D>(filepath);
    }

    Ref<Texture2D> Texture2D::Create(const Ref<Texture2DAsset>& asset)
    {
        return CreateRef<OpenGLTexture2D>(asset);
    }

}