#include "vastpch.h"
#include "BoardTextureSprite.h"

namespace Vast::Board2D {

	TextureSprite::TextureSprite(const Ref<Texture2D>& texture)
		: m_Texture(texture), m_UVs({{ {0.0f, 0.0f}, {1.0f, 1.0f} }})
	{
	}
}