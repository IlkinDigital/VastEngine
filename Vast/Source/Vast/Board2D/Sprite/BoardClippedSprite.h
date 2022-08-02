#pragma once

#include "BoardSprite.h"

namespace Vast::Board2D {

	/**
	* Sprite extracted from a Sprite Sheet
	*/

	class ClippedSprite : public Sprite
	{
	public:
		ClippedSprite(const Ref<SpriteSheet>& spriteSheet, uint16 column, uint16 row);
		ClippedSprite(const Ref<SpriteSheet>& spriteSheet, const SArray<Vector2, 2>& uvs);

		virtual const Ref<Texture2D>& GetTexture() const override;
		virtual SArray<Vector2, 2> GetUVCoords() const override;
		virtual void SetUVCoords(const SArray<Vector2, 2>& coords) override;
	private:
		void CalculateUV(uint16 column, uint16 row);
	private:
		SArray<Vector2, 2> m_UVs;
		Ref<SpriteSheet> m_SpriteSheet;
	};

}
