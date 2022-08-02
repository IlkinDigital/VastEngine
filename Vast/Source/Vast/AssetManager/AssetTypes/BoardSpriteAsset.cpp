#include "vastpch.h"
#include "BoardSpriteAsset.h"

namespace Vast {

    Ref<Asset> BoardSpriteAsset::Clone() const
    {
        auto bsa = CreateRef<BoardSpriteAsset>(m_Name, m_Path, m_UUID);
        bsa->SetSprite(m_Sprite);
        return bsa;
    }

}