#include "vastpch.h"
#include "BoardSpriteSheetAsset.h"

namespace Vast {

    Ref<Asset> BoardSpriteSheetAsset::Clone() const
    {
        auto bssa = CreateRef<BoardSpriteSheetAsset>(m_Name, m_Path, m_UUID);
        bssa->SetSpriteSheet(m_SpriteSheet);
        return bssa;
    }

}