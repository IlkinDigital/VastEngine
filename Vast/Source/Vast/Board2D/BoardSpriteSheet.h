#pragma once

#include "Renderer/Texture2D.h"
#include "Math/Math.h"

#include "Sprite/BoardSprite.h"

namespace Vast::Board2D {

	class SpriteSheet
	{
	public:
		SpriteSheet(const Ref<Texture2D>& sheet);
		SpriteSheet(const Ref<Texture2D>& sheet, const Vector2& stride);

		const Vector2& GetStride() const { return m_Stride; }
		void SetStride(const Vector2& stride) { m_Stride = stride; }

		const Ref<Texture2D>& GetSheet() const { return m_Sheet; }
		void SetSheet(const Ref<Texture2D>& sheet) { m_Sheet = sheet; }
		
		Ref<Sprite> ExtractSprite(uint16 column, uint16 row);
	private:
		Vector2 m_Stride;
		Ref<Texture2D> m_Sheet;
	};

}
