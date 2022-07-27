#include "vastpch.h"
#include "Cubemap.h"

#include "Platform/OpenGL/OpenGLCubemap.h"

namespace Vast {

    Ref<Cubemap> Cubemap::Create(const Filepath& right, const Filepath& left, const Filepath& top, const Filepath& bottom, const Filepath& front, const Filepath& back)
    {
       return CreateRef<OpenGLCubemap>(right, left, top, bottom, front, back);
    }

}