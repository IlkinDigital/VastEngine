#include "vastpch.h"
#include "OpenGLCubemap.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Vast {

    OpenGLCubemap::OpenGLCubemap(const Filepath& right, const Filepath& left, const Filepath& top, const Filepath& bottom, const Filepath& front, const Filepath& back)
    {
        m_Faces[0] = right;
        m_Faces[1] = left;
        m_Faces[2] = top;
        m_Faces[3] = bottom;
        m_Faces[4] = front;
        m_Faces[5] = back;

        Create();
	}

    void OpenGLCubemap::Bind()
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
    }

    void OpenGLCubemap::Create()
    {
        glGenTextures(1, &m_RendererID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

        uint16 size = 6;

        int width, height, channels;
        for (unsigned int i = 0; i < size; i++)
        {
            stbi_set_flip_vertically_on_load(0);
            stbi_uc* data = stbi_load(m_Faces[i].string().c_str(), &width, &height, &channels, 0);
            GLenum internalFormat = 0, dataFormat = 0;
            
            switch (channels)
            {
            case 4:
                internalFormat = GL_RGBA8;
                dataFormat = GL_RGBA;
                break;
            case 3:
                internalFormat = GL_RGB8;
                dataFormat = GL_RGB;
                break;
            default:
                VAST_CORE_ERROR("Invalid number of channels: {0}", channels);
                break;
            }

            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            }
            else
            {
                VAST_CORE_ERROR("Cubemap texture failed to load at path: {0}", m_Faces[i].string());
                stbi_image_free(data);
            }
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
}