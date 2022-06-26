#include "vastpch.h"
#include "RendererInterface.h"

namespace Vast {

    RendererAPI RendererInterface::s_API = RendererAPI::OpenGL;

    RendererAPI RendererInterface::GetRendererAPI()
    {      
        return s_API;
    }

}