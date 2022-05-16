#include "vastpch.h"
#include "OpenGLInterface.h"

#include <glad/glad.h>

namespace Vast {

	void OpenGLInterface::Clear() const
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void OpenGLInterface::SetViewport(float x, float y, float width, float height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLInterface::SetClearColor(const Vector4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	const Vector4& OpenGLInterface::GetClearColor() const
	{
		GLfloat color[4]{};
		glGetFloatv(GL_COLOR_CLEAR_VALUE, color);
		return { color[0], color[1], color[2], color[3] };
	}

	void OpenGLInterface::DrawIndexed(uint32 indexCount) const
	{
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
	}

}