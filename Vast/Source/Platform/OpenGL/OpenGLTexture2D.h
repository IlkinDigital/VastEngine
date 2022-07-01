#pragma once

#include "Renderer/Texture2D.h"

#include <glad/glad.h>

namespace Vast {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32 width, uint32 height);
		OpenGLTexture2D(const Filepath& filepath);
		OpenGLTexture2D(const Ref<TextureAsset>& asset);
		~OpenGLTexture2D() override;

		uint32 GetWidth() const override { return m_Width; }
		uint32 GetHeight() const override { return m_Height; }

		RendererID GetRendererID() const override { return m_RendererID; }
		const Filepath& GetFilepath() const override { return m_Filepath; };

		void SetData(void* data, uint32 size) override;
		void Bind(uint32 slot = 0) const override;
	private:
		void Create(unsigned char* bitmap, int channels);
		// Sets up texture resizing algorithms 
		void SetupFilters() const; 
	private:
		RendererID m_RendererID;
		uint32 m_Width, m_Height;
		GLenum m_DataFormat;

		Filepath m_Filepath;
	};

}