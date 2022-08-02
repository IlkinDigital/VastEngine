#pragma once

#include "Camera/Camera.h"
#include "Camera/EditorCamera.h"
#include "Texture2D.h"
#include "Cubemap.h"

namespace Vast {

	class Renderer2D
	{
	public:
		static void Init();

		static void BeginScene(const Camera& camera, const Mat4& transform);
		static void BeginScene(const EditorCamera& camera);
		static void EndScene();

		static void DrawSkybox(const Ref<Cubemap>& cubemap);

		static void DrawQuad(const Mat4& transform, const Vector4& color);
		static void DrawQuad(const Mat4& transform, const Ref<Texture2D>& texture);
		static void DrawQuad(const Mat4& transform, const Ref<Texture2D>& texture, const SArray<Vector2, 2>& uvCoords);

		static void DrawLine(const Vector3& pt1, const Vector3& pt2, const Vector4& color = { 0.2235f, 1.0f, 0.0784f, 1.0f });
	};

}
