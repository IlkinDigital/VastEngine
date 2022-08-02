#pragma once

#include "BoardSprite.h"

namespace Vast::Board2D {

	class TextureSprite : public Sprite
	{
	public:
		TextureSprite(const Ref<Texture2D>& texture);

		virtual const Ref<Texture2D>& GetTexture() const override { return m_Texture; }
		virtual SArray<Vector2, 2> GetUVCoords() const override { return m_UVs; }
		virtual void SetUVCoords(const SArray<Vector2, 2>& uvs) override { m_UVs = uvs; }
	private:
		Ref<Texture2D> m_Texture;
		SArray<Vector2, 2> m_UVs;
	};

}
