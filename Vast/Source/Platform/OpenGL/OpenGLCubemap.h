#pragma once

#include "CoreMinimal.h"
#include "Renderer/Cubemap.h"

namespace Vast {

	class OpenGLCubemap : public Cubemap
	{
	public:
		OpenGLCubemap(const Filepath& right, const Filepath& left, const Filepath& top, const Filepath& bottom, const Filepath& front, const Filepath& back);
		virtual ~OpenGLCubemap() override = default;

		virtual RendererID GetRendererID() const override { return m_RendererID; }

		virtual void Bind() override;
	private:
		void Create();
	private:
		Filepath m_Faces[6]{};
		RendererID m_RendererID = 0;
	};

}
