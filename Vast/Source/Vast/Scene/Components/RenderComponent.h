#pragma once

#include "Math/Math.h"
#include "Renderer/Texture2D.h"

namespace Vast {

	struct RenderComponent
	{
		Vector4 Color { 0.0f, 0.0f, 0.0f, 1.0f };
		Ref<Texture2D> Texture;

		RenderComponent() = default;
		RenderComponent(const RenderComponent&) = default;
		RenderComponent(const Vector4& color)
			: Color(color) {}
		RenderComponent(const Ref<Texture2D>& texture)
			: Texture(texture) {}
	};

}