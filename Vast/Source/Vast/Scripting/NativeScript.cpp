#include "vastpch.h"
#include "NativeScript.h"

#include "Project/Project.h"
#include "AssetManager/AssetManager.h"
#include "AssetManager/AssetTypes.h"

namespace Vast {

    DArray<Entity> NativeScript::GetEntityByName(const String& name)
    {
        DArray<Entity> entities;

        if (m_Entity.IsValid())
        {
            const auto& view = m_Entity.m_Scene->GetRegistry().view<TagComponent>();

            for (auto entityId : view)
            {
                if (view.get<TagComponent>(entityId).Tag == name)
                {
                    Entity entity(entityId, m_Entity.m_Scene);
                    entities.emplace_back(entity);
                }
            }
        }

        return entities;
    }

    Entity NativeScript::CreateEntity(const String& name)
    {
        return m_Entity.m_Scene->CreateEntity(name);
    }

    void NativeScript::DestroySelf()
    {
        OnDestroy();
        m_Entity.m_Scene->DestroyEntity(m_Entity);
        m_Destroyed = true;
    }
}
