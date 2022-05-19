#pragma once

#include "Renderer/Framebuffer.h"

namespace Vast {

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		~OpenGLFramebuffer() override;

		const FramebufferSpecification& GetSpecification() const override { return m_Specification; }
		RendererID GetColorAttachment() const override { return m_ColorAttachment; }

		void Resize(const FramebufferSpecification& spec) override;

		void Bind() const override;
		void Unbind() const override;

		void Generate();
	private:
		RendererID m_RendererID;
		RendererID m_ColorAttachment, m_DepthAttachment;
		FramebufferSpecification m_Specification;
	};

}
