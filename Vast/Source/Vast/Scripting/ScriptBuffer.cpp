#include "vastpch.h"
#include "ScriptBuffer.h"

namespace Vast {

    Scope<ScriptBuffer> ScriptBuffer::s_Instance = CreateScope<ScriptBuffer>();

    ScriptBuffer::ScriptBuffer()
    {
        if (!s_Instance)
            s_Instance = Scope<ScriptBuffer>(this);
    }

    Scope<NativeScriptComponent> ScriptBuffer::FindByName(const String& name)
    {
        for (auto& nsc : m_Buffer)
        {
            if (nsc.Name == name)
                return CreateScope<NativeScriptComponent>(nsc);
        }
        
        VAST_CORE_ERROR("Script with name '{0}' doesn't exist", name);
        return nullptr;
    }

}