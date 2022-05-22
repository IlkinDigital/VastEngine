#pragma once

#include "Renderer/RendererInterface.h"

namespace Vast {

	class OpenGLInterface : public RendererInterface
	{
	public:
		OpenGLInterface() = default;

		void Init() override;

		void SetViewport(int32 x, int32 y, uint32 width, uint32 height) override;
		
		void SetClearColor(const Vector4& color) override;
		Vector4 GetClearColor() const override;
		
		void Clear() override;
		void DrawIndexed(uint32 indexCount) const override;
	};
}
