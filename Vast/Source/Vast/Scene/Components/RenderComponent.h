#pragma once

#include "Math/Math.h"
#include "Renderer/Texture2D.h"

namespace Vast {

	enum class RenderCategory { Color, Texture };

	struct RenderComponent
	{
		RenderCategory Category;
		Vector4 Color;
		Ref<Texture2D> Texture;

		RenderComponent() = default;
		RenderComponent(const RenderComponent&) = default;
		RenderComponent(const Vector4& color)
			: Color(color), Category(RenderCategory::Color) {}
		RenderComponent(const Ref<Texture2D>& texture)
			: Texture(texture), Category(RenderCategory::Texture) {}
	};

}