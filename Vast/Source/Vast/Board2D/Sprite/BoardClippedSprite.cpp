#include "vastpch.h"
#include "BoardClippedSprite.h"

#include "Board2D/BoardSpriteSheet.h"

namespace Vast::Board2D {

    ClippedSprite::ClippedSprite(const Ref<SpriteSheet>& spriteSheet, uint16 column, uint16 row)
        : m_SpriteSheet(spriteSheet) 
    {
        CalculateUV(column, row);
    }

    ClippedSprite::ClippedSprite(const Ref<SpriteSheet>& spriteSheet, const SArray<Vector2, 2>& uvs)
        : m_SpriteSheet(spriteSheet), m_UVs(uvs) {}

    const Ref<Texture2D>& ClippedSprite::GetTexture() const
    {
        return m_SpriteSheet->GetSheet();
    }

    SArray<Vector2, 2> ClippedSprite::GetUVCoords() const
    {
        return m_UVs;
    }

    void ClippedSprite::SetUVCoords(const SArray<Vector2, 2>& coords)
    {
        m_UVs = coords;
    }

    void ClippedSprite::CalculateUV(uint16 column, uint16 row)
    {
        Ref<Texture2D> sheet = m_SpriteSheet->GetSheet();

        float strideX = m_SpriteSheet->GetStride().x / (float)sheet->GetWidth();
        float strideY = m_SpriteSheet->GetStride().y / (float)sheet->GetHeight();

        m_UVs[0] = { strideX * column, strideY * row };
        m_UVs[1] = { strideX * (column + 1), strideY * (row + 1) };
    }

}