#pragma once

#include "Renderer/RendererInterface.h"

namespace Vast {

	class OpenGLInterface : public RendererInterface
	{
	public:
		OpenGLInterface() = default;

		virtual void Init() override;

		virtual void SetViewport(int32 x, int32 y, uint32 width, uint32 height) override;

		virtual void SetDepthMask(bool flag) const override;
		
		virtual void SetClearColor(const Vector4& color) override;
		virtual Vector4 GetClearColor() const override;
		
		virtual void Clear() override;
		virtual void DrawIndexed(uint32 indexCount) const override;
		virtual void DrawLines(uint32 vertexCount) const override;
	};
}
