#include "vastpch.h"
#include "SceneRenderer.h"

#include "RendererCommand.h"

namespace Vast {

	void SceneRenderer::Init(uint32 width, uint32 height)
	{
		OPTICK_EVENT();

		m_Framebuffer = Framebuffer::Create({ width, height });
		RendererCommand::SetClearColor({ 0.15f, 0.15f, 0.15f, 1.0f });
	}

	void SceneRenderer::Begin()
	{
		OPTICK_EVENT();

		m_Framebuffer->Bind();
		RendererCommand::Clear();
	}

	void SceneRenderer::End()
	{
		OPTICK_EVENT();

		m_Framebuffer->Unbind();
	}
}