#pragma once

#include "Math/Math.h"
#include "Renderer/Texture2D.h"

#include "Board2D/Sprite/BoardSprite.h"

namespace Vast {
	
	struct RenderObject
	{
		virtual const Vector4& GetColor() const = 0;
		virtual const Ref<Texture2D>& GetTexture() const = 0;
		virtual const SArray<Vector2, 2>& GetTextureCoords() const = 0;
	};

	struct RenderComponent : public RenderObject
	{
		Vector4 Color { 0.0f, 0.0f, 0.0f, 1.0f };
		SArray<Vector2, 2> TextureCoords = { { {0.0f, 0.0f}, {1.0f, 1.0f} } };
		Ref<Texture2D> Texture;

		RenderComponent() = default;
		RenderComponent(const RenderComponent&) = default;
		RenderComponent(const Vector4& color)
			: Color(color) {}
		RenderComponent(const Ref<Texture2D>& texture)
			: Texture(texture) {}

		virtual const Vector4& GetColor() const override { return Color; }
		virtual const Ref<Texture2D>& GetTexture() const override { return Texture; }
		virtual const SArray<Vector2, 2>& GetTextureCoords() const override { return TextureCoords; }
	};

	struct BoardRenderComponent : public RenderObject
	{
		Vector4 Color = Vector4(1.0f);
		Ref<Board2D::Sprite> Sprite;

		BoardRenderComponent() = default;
		BoardRenderComponent(const BoardRenderComponent&) = default;
		BoardRenderComponent(const Ref<Board2D::Sprite>& sprite)
			: Sprite(sprite) {}

		virtual const Vector4& GetColor() const override { return Color; }
		virtual const Ref<Texture2D>& GetTexture() const override { return Sprite ? Sprite->GetTexture() : nullptr; }
		virtual const SArray<Vector2, 2>& GetTextureCoords() const override { return Sprite->GetUVCoords(); }
	};

}