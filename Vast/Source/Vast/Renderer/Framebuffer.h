#pragma once

#include "RendererCore.h"

namespace Vast {

	struct FramebufferSpecification
	{
		uint32 Width, Height;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual const FramebufferSpecification& GetSpecification() const = 0;
		virtual RendererID GetColorAttachment() const = 0;

		virtual void Resize(const FramebufferSpecification& spec) = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& specification);
	};

}