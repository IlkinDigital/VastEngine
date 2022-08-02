#pragma once

#include "CoreMinimal.h"
#include "Math/Math.h"
#include "Renderer/Texture2D.h"

namespace Vast::Board2D {

	class SpriteSheet;

	class Sprite
	{
	public:
		virtual const Ref<Texture2D>& GetTexture() const = 0;
		virtual void SetUVCoords(const SArray<Vector2, 2>& coords) = 0;
		virtual SArray<Vector2, 2> GetUVCoords() const = 0;

		static Ref<Sprite> Create(const Ref<SpriteSheet>& spriteSheet, uint16 column, uint16 row);
		static Ref<Sprite> Create(const Ref<SpriteSheet>& spriteSheet, const SArray<Vector2, 2>& uvs = {{ {0.0f, 0.0f}, {1.0f, 1.0f} }});
		static Ref<Sprite> Create(const Ref<Texture2D>& texture);
	};

}