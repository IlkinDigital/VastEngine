#pragma once

#include "Renderer/Shader.h"

namespace Vast {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const String& name, const String& vertexSource, const String& fragSource);
		~OpenGLShader() override;

		void Bind() const override;
		void Unbind() const override;
	
		const String& GetName() const override { return m_Name; }
	private:
		void Compile(const String& vertexSource, const String& fragSource);
	private:
		String m_Name;

		RendererID m_Program;
	};
}
