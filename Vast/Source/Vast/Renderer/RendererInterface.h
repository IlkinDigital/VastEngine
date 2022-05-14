#pragma once

#include "Renderer/RendererCore.h"
#include "Core/Math/Math.h"

namespace Vast {

	class RendererInterface
	{
	public:
		virtual void SetClearColor(const Vector4& color) = 0;
		virtual const Vector4& GetClearColor() const = 0;

		virtual void Clear() const = 0;
		virtual void DrawIndexed(uint32 indexCount) const = 0;

		static RendererAPI GetRendererAPI();
	private:
		static RendererAPI s_API;
	};

}