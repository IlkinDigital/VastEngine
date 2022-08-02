#include "vastpch.h"
#include "BoardSpriteSheet.h"

namespace Vast::Board2D {

	SpriteSheet::SpriteSheet(const Ref<Texture2D>& sheet)
		: m_Sheet(sheet)
	{
		m_Stride = { sheet->GetWidth(), sheet->GetHeight() };
	}

	SpriteSheet::SpriteSheet(const Ref<Texture2D>& sheet, const Vector2& stride)
		: m_Sheet(sheet), m_Stride(stride)
	{
	}

	Ref<Sprite> SpriteSheet::ExtractSprite(uint16 column, uint16 row)
	{
		return Sprite::Create(Ref<SpriteSheet>(this), column, row);
	}

}