#pragma once

#include "Cameras/Camera.h"

namespace Vast {

	class Renderer2D
	{
	public:
		static void Init();

		static void BeginScene(const Camera& camera, const Mat4& transform);
		static void EndScene();

		static void DrawQuad(const Mat4& transform, const Vector4& color);
	};

}
