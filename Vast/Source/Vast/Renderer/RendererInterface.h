#pragma once

#include "Renderer/RendererCore.h"
#include "Renderer/VertexArray.h"
#include "Core/Math/Math.h"

namespace Vast {

	class RendererInterface
	{
	public:
		virtual void Init() = 0;

		virtual void SetViewport(int32 x, int32 y, uint32 width, uint32 height) = 0;

		virtual void SetDepthMask(bool flag) const = 0;

		virtual void SetClearColor(const Vector4& color) = 0;
		virtual Vector4 GetClearColor() const = 0;

		virtual void Clear() = 0;
		virtual void DrawIndexed(uint32 indexCount) const = 0;
		virtual void DrawLines(uint32 vertexCount) const = 0;

		static RendererAPI GetRendererAPI();
	private:
		static RendererAPI s_API;
	};

}