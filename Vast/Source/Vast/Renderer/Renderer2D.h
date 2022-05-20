#pragma once

#include "Camera/Camera.h"
#include "Texture2D.h"

namespace Vast {

	class Renderer2D
	{
	public:
		static void Init();

		static void BeginScene(const Camera& camera, const Mat4& transform);
		static void EndScene();

		static void DrawQuad(const Mat4& transform, const Vector4& color);
		static void DrawQuad(const Mat4& transform, const Ref<Texture2D>& texture);
	};

}
