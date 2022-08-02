#include "vastpch.h"
#include "BoardSprite.h"

#include "Board2D/BoardSpriteSheet.h"
#include "BoardClippedSprite.h"
#include "BoardTextureSprite.h"

namespace Vast::Board2D {

    Ref<Sprite> Sprite::Create(const Ref<SpriteSheet>& spriteSheet, uint16 column, uint16 row)
    {
        return CreateRef<ClippedSprite>(spriteSheet, column, row);
    }

    Ref<Sprite> Sprite::Create(const Ref<SpriteSheet>& spriteSheet, const SArray<Vector2, 2>& uvs)
    {
        return CreateRef<ClippedSprite>(spriteSheet, 0, 0);
    }

    Ref<Sprite> Sprite::Create(const Ref<Texture2D>& texture)
    {
        return CreateRef<TextureSprite>(texture);
    }

}