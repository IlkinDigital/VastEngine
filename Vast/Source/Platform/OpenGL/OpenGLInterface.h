#pragma once

#include "Renderer/RendererInterface.h"

namespace Vast {

	class OpenGLInterface : public RendererInterface
	{
	public:
		OpenGLInterface() = default;

		void SetViewport(float x, float y, float width, float height) override;
		
		void SetClearColor(const Vector4& color) override;
		const Vector4& GetClearColor() const override;
		
		void Clear() const override;
		void DrawIndexed(uint32 indexCount) const override;
	};

}
