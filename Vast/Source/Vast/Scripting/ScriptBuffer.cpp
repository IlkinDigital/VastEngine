#include "vastpch.h"
#include "ScriptBuffer.h"

namespace Vast {

    const NativeScriptComponent& ScriptBuffer::FindByName(const String& name) const
    {
        for (auto& nsc : m_Buffer)
        {
            if (nsc.Name == name)
                return nsc;
        }
        
        VAST_CORE_ERROR("Script with name '{0}' doesn't exist", name);
        
        NativeScriptComponent null;
        return null;
    }

}