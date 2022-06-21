#include "vastpch.h"
#include "ScriptableEntity.h"

namespace Vast {

    DArray<Entity> ScriptableEntity::GetEntityByName(const String& name)
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

    Entity ScriptableEntity::CreateEntity(const String& name)
    {
        return m_Entity.m_Scene->CreateEntity(name);
    }

}