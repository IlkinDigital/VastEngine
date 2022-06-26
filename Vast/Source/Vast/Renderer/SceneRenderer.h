#pragma once

#include <Vast/Renderer/Framebuffer.h>

namespace Vast {

	class SceneRenderer
	{
	public:
		const Ref<Framebuffer> GetFramebuffer() const { return m_Framebuffer; }

		void Init(uint32 width, uint32 height);
		void Begin();
		void End();
	private:
		Ref<Framebuffer> m_Framebuffer;
	};

}