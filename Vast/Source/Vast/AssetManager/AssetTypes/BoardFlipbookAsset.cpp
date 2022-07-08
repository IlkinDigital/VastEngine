#include "vastpch.h"
#include "BoardFlipbookAsset.h"

namespace Vast {

    Ref<Asset> BoardFlipbookAsset::Clone() const
    {
        Ref<BoardFlipbookAsset> bfa = CreateRef<BoardFlipbookAsset>(m_Name, m_Path, m_UUID);
        bfa->m_Flipbook = CreateRef<Board2D::Flipbook>(*m_Flipbook);
        return bfa;
    }

}