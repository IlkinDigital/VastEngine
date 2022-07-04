#include "vastpch.h"
#include "OpenGLTexture2D.h"

#include "AssetManager/Texture2DAsset.h"

#include <stb_image.h>

namespace Vast {

	OpenGLTexture2D::OpenGLTexture2D(uint32 width, uint32 height)
		: m_Width(width), m_Height(height), m_DataFormat(GL_RGBA)
	{
		OPTICK_EVENT();

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, GL_RGBA8, m_Width, m_Height);

		SetupFilters();
	}

	OpenGLTexture2D::OpenGLTexture2D(const Filepath& filepath)
		: m_Path(filepath)
	{
		OPTICK_EVENT();

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		
		stbi_uc* bitmap = nullptr;
		bitmap = stbi_load(filepath.string().c_str(), &width, &height, &channels, 0);

		m_Width = width;
		m_Height = height;

		Create(bitmap, channels);
	}

	OpenGLTexture2D::OpenGLTexture2D(const Ref<Texture2DAsset>& asset)
	{
		OPTICK_EVENT();

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);

		stbi_uc* bitmap = nullptr;
		const char* data = asset->GetFileData().data();
		bitmap = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(data), asset->GetFileData().size(), &width, &height, &channels, 0);
		m_Width = width;
		m_Height = height;

		Create(bitmap, channels);

		m_Path = asset->GetPath();
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32 size)
	{
		OPTICK_EVENT();

		// Bytes Per Pixel
		uint16 bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		VAST_CORE_ASSERT(m_Width * m_Height * bpp == size, "Data must cover the entire texture");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32 slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

	void OpenGLTexture2D::Create(unsigned char* bitmap, int channels)
	{
		OPTICK_EVENT();

		VAST_CORE_ASSERT(bitmap, "Couldn't load texture file");

		GLenum internalFormat = 0, dataFormat = 0;

		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		VAST_CORE_ASSERT((internalFormat & dataFormat), "NOT supported image format specified");

		m_DataFormat = dataFormat;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		SetupFilters();

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, bitmap);

		stbi_image_free(bitmap);
	}

	void OpenGLTexture2D::SetupFilters() const
	{
		OPTICK_EVENT();

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

}