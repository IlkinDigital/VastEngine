#include "ScriptBuffer.h"

namespace Vast {

    NativeScriptComponent& ScriptBuffer::GetByName(const String& name)
    {
        for (auto& nsc : m_Buffer)
        {
            if (nsc.Name == name)
                return nsc;
        }
        
        VAST_ASSERT(false, "Script with specified name doesn't exist");
    }

}