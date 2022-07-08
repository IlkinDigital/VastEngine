#include "vastpch.h"
#include "SceneAsset.h"

namespace Vast {

    Ref<Asset> Vast::SceneAsset::Clone() const
    {
       auto sa = CreateRef<SceneAsset>(m_Name, m_Path, m_UUID);
       sa->SetScene(m_Scene->Clone(m_Scene));
       return sa;
    }

}