#pragma once

#include "Renderer/Texture2D.h"

#include <glad/glad.h>

namespace Vast {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32 width, uint32 height);
		OpenGLTexture2D(const String& filepath);
		~OpenGLTexture2D() override;

		uint32 GetWidth() const override { return m_Width; }
		uint32 GetHeight() const override { return m_Height; }

		void SetData(void* data, uint32 size) override;
		void Bind(uint32 slot = 0) const override;
	private:
		// Sets up texture resizing algorithms 
		void SetupResizing() const; 
	private:
		RendererID m_RendererID;
		uint32 m_Width, m_Height;
		GLenum m_DataFormat;
	};

}